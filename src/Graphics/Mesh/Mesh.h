#ifndef MESH_H
#define MESH_H

#include <string>

#include "Vector3.h"
#include "Vector2.h"

using namespace std;


class Edge {

public:

  unsigned short v0, v1;
  int f;

  Edge() {}
  Edge(unsigned short v0, unsigned short v1, int f) : v0(v0), v1(v1), f(f) {}

};


class EdgeCompare {

public:

  bool operator() (const Edge &a, const Edge &b) {

    unsigned short a0 = min(a.v0, a.v1);
    unsigned short a1 = max(a.v0, a.v1);
    unsigned short b0 = min(b.v0, b.v1);
    unsigned short b1 = max(b.v0, b.v1);

    if (a0 != b0)
      return a0 < b0;

    return a1 < b1;

  }

};


class EdgePair {

public:

  int e0, e1;

  EdgePair() {}
  EdgePair(int e0, int e1) : e0(e0), e1(e1) {}

};


class Mesh {

public:

  int numVerts;
  int numIndices;
  int numEdgePairs;

  Vector3f *verts;
  Vector2f *vertUVs;
  Vector3f *vertNormals;
  Vector3f *faceNormals;

  Edge     *edges;
  EdgePair *edgePairs;

  // maps vertex index to unique vertex index, since some vertices are repeated with
  // different UV or normals
  unsigned short *uniqueVertIndex;

  unsigned short *indices;

public:

  Mesh() {

    verts = NULL;
    vertUVs = NULL;
    vertNormals = NULL;
    faceNormals = NULL;

    edges = NULL;
    edgePairs = NULL;

    uniqueVertIndex = NULL;

    indices = NULL;

    numVerts = 0;
    numIndices = 0;
    numEdgePairs = 0;

  }

  ~Mesh() {
    cleanup();
  }

  void cleanup() {

    if (verts)
      delete[] verts;

    if (vertUVs)
      delete[] vertUVs;

    if (vertNormals)
      delete[] vertNormals;

    if (faceNormals)
      delete[] faceNormals;


    if (edges)
      delete[] edges;

    if (edgePairs)
      delete[] edgePairs;


    if (uniqueVertIndex)
      delete[] uniqueVertIndex;


    if (indices)
      delete[] indices;


    numVerts = numIndices = numEdgePairs = 0;

  }

  int getNumVerts(void) const {
    return numVerts;
  }

  int getNumIndices(void) const {
    return numIndices;
  }

  int getNumTri(void) const {
    return numIndices / 3;
  }

  int getNumEdgePairs(void) const {
    return numEdgePairs;
  }

  const Vector3f* getVerts(void) const {
    return verts;
  }

  const Vector2f* getVertUVs(void) const {
    return vertUVs;
  }

  const Vector3f* getVertNormals(void) const {
    return vertNormals;
  }

  const Vector3f* getFaceNormals(void) const {
    return faceNormals;
  }

  const Edge* getEdges(void) const {
    return edges;
  }

  const EdgePair* getEdgePairs(void) const {
    return edgePairs;
  }

  const unsigned short* getIndices(void) const {
    return indices;
  }


  bool loadOBJ(const string &filename);
  void createBox(const Vector3f &scale);

  void calcFaceNormals();
  void calcEdgeInfo();

  void dump();

  void render();

};



#endif // MESH_H
