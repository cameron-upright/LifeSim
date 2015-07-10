#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string>

#include "Mesh.h"

#include "Vector2.h"
#include "Vector3.h"

using namespace std;


vector<string> splitString(string str, string separators) {

  vector<string> tokens;

  size_t index0 = str.find_first_not_of(separators);
  size_t index1 = str.find_first_of(separators, index0);

  while (index0 != string::npos) {

    tokens.push_back(str.substr(index0, index1-index0));

    index0 = str.find_first_not_of(separators, index1);
    index1 = str.find_first_of(separators, index0);

  } 

  return tokens;
}


vector<string> splitStringIncludeEmpty(string str, string separators) {

  vector<string> tokens;

  size_t index0 = 0;
  size_t index1 = 0;


  while (index1 != string::npos) {

    index1 = str.find_first_of(separators, index0);

    tokens.push_back(str.substr(index0, index1-index0));

    index0 = index1+1;

  } 

  return tokens;

}


bool Mesh::loadOBJ(const string &filename) {

  vector<string> tokens;

  // temporary storage for the data we load
  vector<Vector3f> tmpVerts;
  vector<Vector2f> tmpVertUVs;
  vector<Vector3f> tmpVertNormals;
  vector<unsigned short> tmpTris;

  // A map between face triplet and final vertex index
  map<Vector3i, unsigned short> tripletMap;

  // Stores the triplet info for all the final vertices
  vector<Vector3i> tmpTriplets;

  // Stuff for reading the file
  ifstream f(filename.c_str());
  string line;

  // Keeps track of if there are any UV or Normals
  bool hasNormals = true;
  bool hasUVs = true;

	cleanup();

  if (!f.is_open())
    return false;

  while (f.good()) {
    
    getline (f,line);

    if (line[0] == '#')
      continue;

    // split on spaces
    vector<string> tokens = splitString(line, string(" "));

    if (tokens.size() == 0)
      continue;

    if (tokens[0] == "v") {
      tmpVerts.push_back(Vector3f(atof(tokens[1].c_str()),
																	atof(tokens[2].c_str()),
																	atof(tokens[3].c_str())));
    }

    else if (tokens[0] == "vn") {
      tmpVertNormals.push_back(Vector3f(atof(tokens[1].c_str()),
																				atof(tokens[2].c_str()),
																				atof(tokens[3].c_str())));
    }

    else if (tokens[0] == "vt") {
      tmpVertUVs.push_back(Vector2f(atof(tokens[1].c_str()),
																		atof(tokens[2].c_str())));
    }

    else if (tokens[0] == "f") {
      for (int i=1; i<4; i++) {

				vector<string> faceTokens = splitStringIncludeEmpty(tokens[i], string("/"));

				// get the vertex, uv and face indices
				int vertIndex;
				int uvIndex;
				int normIndex;

				vertIndex = atoi(faceTokens[0].c_str()) - 1;

				if (faceTokens.size() > 1) {
					uvIndex = faceTokens[1].empty() ? -1 : atoi(faceTokens[1].c_str()) - 1;
					normIndex = faceTokens[2].empty() ? -1 : atoi(faceTokens[2].c_str()) - 1;
				} else
					uvIndex = normIndex = -1;

				hasNormals &= normIndex >= 0;
				hasUVs     &= uvIndex   >= 0;

				// find the unique vertex index
				Vector3i faceTriplet(vertIndex, uvIndex, normIndex);

				// This triplet has not been encountered yet
				if (tripletMap.count(faceTriplet) == 0) {

					int finalVertIndex = tripletMap.size();

					tripletMap[faceTriplet] = finalVertIndex;

					tmpTriplets.push_back(faceTriplet);

				}

				tmpTris.push_back(tripletMap[faceTriplet]);
				//				cout << tripletMap[faceTriplet] << " : " << vertIndex << " " << uvIndex << " " << normIndex << endl;

      }

    }

  }

  f.close();


  numVerts = tmpTriplets.size();

  // Create and populate the vertex array
  verts = new Vector3f[numVerts];
  for (unsigned i=0; i<tmpTriplets.size(); i++)
    verts[i] = tmpVerts[tmpTriplets[i][0]];

  // Create and populate the UV array
  if (hasUVs) {
    vertUVs = new Vector2f[numVerts];
    for (unsigned i=0; i<tmpTriplets.size(); i++)
      vertUVs[i] = tmpVertUVs[tmpTriplets[i][1]];
  }

  // Create and populate the normal array
  if (hasNormals) {
    vertNormals = new Vector3f[numVerts];
    for (unsigned i=0; i<tmpTriplets.size(); i++)
      vertNormals[i] = tmpVertNormals[tmpTriplets[i][2]];
  }

  // Create the index array
  numIndices = tmpTris.size();
	indices = new unsigned short[numIndices];
  for (unsigned i=0; i<tmpTris.size(); i++)
    indices[i] = tmpTris[i];

	calcFaceNormals();

	calcEdgeInfo();

	/*
  for (int i=0; i<numVerts; i++)
    cout << verts[i][0] << "," << verts[i][1] << "," << verts[i][2] << endl;
	cout << endl;

  for (int i=0; i<numVerts; i++)
    cout << vertNormals[i][0] << "," << vertNormals[i][1] << "," << vertNormals[i][2] << endl;
	cout << endl;

  for (int i=0; i<numIndices; i++)
    cout << indices[i] << endl;
	*/

  return true;

}

void Mesh::createBox(const Vector3f &scale) {

	cleanup();

	float a = scale[0];
	float b = scale[1];
	float c = scale[2];

	numVerts = 24;
	numIndices = 36;

	verts = new Vector3f[numVerts];
	vertNormals = new Vector3f[numVerts];
	indices = new unsigned short[numIndices];

  verts[0]  = Vector3f( a,-b,-c);
  verts[1]  = Vector3f( a,-b, c);
  verts[2]  = Vector3f(-a,-b, c);
  verts[3]  = Vector3f(-a,-b,-c);
  verts[4]  = Vector3f( a, b,-c);
  verts[5]  = Vector3f(-a, b,-c);
  verts[6]  = Vector3f(-a, b, c);
  verts[7]  = Vector3f( a, b, c);
  verts[8]  = Vector3f( a,-b,-c);
  verts[9]  = Vector3f( a, b,-c);
  verts[10] = Vector3f( a, b, c);
  verts[11] = Vector3f( a,-b, c);
  verts[12] = Vector3f( a,-b, c);
  verts[13] = Vector3f( a, b, c);
  verts[14] = Vector3f(-a, b, c);
  verts[15] = Vector3f(-a,-b, c);
  verts[16] = Vector3f(-a,-b, c);
  verts[17] = Vector3f(-a, b, c);
  verts[18] = Vector3f(-a, b,-c);
  verts[19] = Vector3f(-a,-b,-c);
  verts[20] = Vector3f( a, b,-c);
  verts[21] = Vector3f( a,-b,-c);
  verts[22] = Vector3f(-a,-b,-c);
  verts[23] = Vector3f(-a, b,-c);

  vertNormals[0]  = Vector3f( 0,-1, 0);
  vertNormals[1]  = Vector3f( 0,-1, 0);
  vertNormals[2]  = Vector3f( 0,-1, 0);
  vertNormals[3]  = Vector3f( 0,-1, 0);
  vertNormals[4]  = Vector3f( 0, 1, 0);
  vertNormals[5]  = Vector3f( 0, 1, 0);
  vertNormals[6]  = Vector3f( 0, 1, 0);
  vertNormals[7]  = Vector3f( 0, 1, 0);
  vertNormals[8]  = Vector3f( 1, 0, 0);
  vertNormals[9]  = Vector3f( 1, 0, 0);
  vertNormals[10] = Vector3f( 1, 0, 0);
  vertNormals[11] = Vector3f( 1, 0, 0);
  vertNormals[12] = Vector3f( 0, 0, 1);
  vertNormals[13] = Vector3f( 0, 0, 1);
  vertNormals[14] = Vector3f( 0, 0, 1);
  vertNormals[15] = Vector3f( 0, 0, 1);
  vertNormals[16] = Vector3f(-1, 0, 0);
  vertNormals[17] = Vector3f(-1, 0, 0);
  vertNormals[18] = Vector3f(-1, 0, 0);
  vertNormals[19] = Vector3f(-1, 0, 0);
  vertNormals[20] = Vector3f( 0, 0,-1);
  vertNormals[21] = Vector3f( 0, 0,-1);
  vertNormals[22] = Vector3f( 0, 0,-1);
  vertNormals[23] = Vector3f( 0, 0,-1);

	indices[0]  = 0;
	indices[1]  = 1;
	indices[2]  = 2;
	indices[3]  = 0;
  indices[4]  = 2;
  indices[5]  = 3;
	indices[6]  = 4;
  indices[7]  = 5;
  indices[8]  = 6;
  indices[9]  = 4;
  indices[10] = 6;
  indices[11] = 7;
	indices[12] = 8;
  indices[13] = 9;
	indices[14] = 10;
  indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;

	calcFaceNormals();

	calcEdgeInfo();

	//	dump();

}

void Mesh::calcFaceNormals() {

	faceNormals = new Vector3f[getNumTri()];

	for (int i=0; i<getNumTri(); i++) {
		Vector3f e0 = verts[indices[i*3+1]]-verts[indices[i*3]];
		Vector3f e1 = verts[indices[i*3+2]]-verts[indices[i*3]];

		faceNormals[i] = cross(e0, e1).normalized();
		//		cout << faceNormals[i] << endl;
	}

}

void Mesh::calcEdgeInfo() {

	map<Vector3f, unsigned short> vertexMap;

	uniqueVertIndex = new unsigned short[numVerts];

	for (int i=0; i<numVerts; i++) {

		// haven't encountered this vertex before
		// add a new unique vertex to the map
		if (vertexMap.find(verts[i]) == vertexMap.end()) {
			unsigned short newIndex = vertexMap.size();
			vertexMap[verts[i]] = newIndex;
		}

		uniqueVertIndex[i] = vertexMap[verts[i]];

	}



	// Find all the edges
	edges = new Edge[numIndices];

	// For every triangle
	for (int i=0; i<numIndices/3; i++)
		for (int j=0; j<3; j++)
			edges[i*3+j] = Edge(indices[i*3+j%3], indices[i*3+(j+1)%3], i);


	map<Edge, int, EdgeCompare> edgeMap;
	vector<EdgePair> tmpEdgePairs;

	// For every edge
	for (int i=0; i<numIndices/3; i++) {
		for (int j=0; j<3; j++) {

			Edge edge = edges[i*3+j];
			Edge uniqueVertexEdge = Edge(uniqueVertIndex[edge.v0], uniqueVertIndex[edge.v1], edge.f);

			map<Edge, int, EdgeCompare>::iterator it = edgeMap.find(uniqueVertexEdge);

			if (it != edgeMap.end()) {
				tmpEdgePairs.push_back(EdgePair(it->second, i*3+j));
				edgeMap.erase(it);
			} else
				edgeMap[uniqueVertexEdge] = i*3+j;

		}
	}

	numEdgePairs = tmpEdgePairs.size();
	edgePairs = new EdgePair[numEdgePairs];

	// Save the edge pairs
	for (int i=0; i<numEdgePairs; i++)
		edgePairs[i] = tmpEdgePairs[i];


	/*
	for (int i=0; i<numVerts; i++)
		cout << uniqueVertIndex[i] << " ";
	cout << endl;

	map<Edge, int, EdgeCompare> edgeFaceMap;
	vector<EdgePair> tmpEdgePairs;

	// For every triangle
	for (int i=0; i<numIndices/3; i++) {

		for (int j=0; j<3; j++) {

			// We use unique vertex indices here, since identical vertices can have different indices, with unique normals and UVs
			Edge edge(uniqueVertIndex[indices[i*3+j%3]], uniqueVertIndex[indices[i*3+(j+1)%3]]);

			// try finding the edges pair
			map<Edge, int, EdgeCompare>::iterator it = edgeFaceMap.find(edge);

			// the other edge exists
			if (it != edgeFaceMap.end()) {

				tmpEdgePairs.push_back(EdgePair(it->first, edgeFaceMap[it->first], i));

				cout << it->first.v0 << " " << it->first.v1 << " " << edgeFaceMap[it->first] << " " << i << endl;

				edgeFaceMap.erase(it);

			} else

				edgeFaceMap[edge] = i;

		}

	}


	*/

}

void Mesh::dump() {

	cout << "VERTS" << endl;
	for (int i=0; i<numVerts; i++)
		cout << verts[i] << endl;
	cout << endl;

	cout << "TRIS" << endl;
	for (int i=0; i<numIndices/3; i++) {
		for (int j=0; j<3; j++)
			cout << indices[i*3+j] << " ";
		cout << endl;
	}
	cout << endl;

	cout << "UNIQUE VERTS" << endl;
	for (int i=0; i<numIndices/3; i++) {
		for (int j=0; j<3; j++)
			cout << uniqueVertIndex[indices[i*3+j]] << " ";
		cout << endl;
	}
	cout << endl;


	cout << "EDGES" << endl;
	for (int i=0; i<numIndices; i++) {
		cout << edges[i].v0 << " " << edges[i].v1 << endl;
	}
	cout << endl;


	cout << "EDGE PAIRS" << endl;
	for (int i=0; i<numEdgePairs; i++) {
		cout << edgePairs[i].e0 << " " << edgePairs[i].e1 << " " << edges[edgePairs[i].e0].f << " " << edges[edgePairs[i].e1].f << endl;
	}
	cout << endl;


}


void Mesh::render() {

	if (numVerts == 0 || numIndices == 0)
		return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);

	if (vertNormals) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, vertNormals);
	}

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);


	/*
	glBegin(GL_LINES);

	for (int i=0; i<getNumTri(); i++) {
		Vector3f x0 = (verts[indices[i*3]] + verts[indices[i*3+1]] + verts[indices[i*3+2]]) / 3.0f;
		Vector3f x1 = x0 + faceNormals[i];

		glVertex3fv(x0.get_ptr());
		glVertex3fv(x1.get_ptr());
	}

	glEnd();
	*/

}


