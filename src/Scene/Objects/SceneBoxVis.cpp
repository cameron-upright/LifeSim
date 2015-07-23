#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include "SceneBoxVis.h"

#include "Transform.h"
#include "Vector4.h"

void SceneBoxVis::render(bool ambientPass) {

  glPushMatrix();

  // Get the transform
  btTransform trans;
  box->getMotionState()->getWorldTransform(trans);

  float m[16];
  trans.getOpenGLMatrix(m);

	glMultMatrixf(m);

	material->applyMaterial();

	// Use the vertex program and fragment program
	if (vertexProgram && fragmentProgram && !ambientPass) {

		glEnable(GL_VERTEX_PROGRAM_ARB);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgram->vertexProgram);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragmentProgram->fragmentProgram);

	}

  mesh.render();

	if (vertexProgram && fragmentProgram) {

		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, 0);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, 0);

	}



  glPopMatrix();

}

void SceneBoxVis::calcLightFacing(const Vector4f &lightPosition) {

  btTransform transform;
  box->getMotionState()->getWorldTransform(transform);

  Quaternionf q(transform.getRotation());
  Vector3f p(transform.getOrigin());

  for (int i=0; i<mesh.getNumTri(); i++) {

    // The first vertex of the triangle in world space
    Vector3f x = q * mesh.getVerts()[mesh.getIndices()[i*3]] + p;

    // The vertex normal transformed into world space
    Vector3f n = q * mesh.getFaceNormals()[i];

    Vector3f l = Vector3f(lightPosition[0], lightPosition[1], lightPosition[2]) - x * lightPosition[3];

		//		cout << mesh.getVerts() << endl;

		/*
		if (i==0) {
			//			cout << n << " = " << q << "*" << mesh.getFaceNormals()[i] << " " << l << endl;
			//			cout << n[0] << " " << n[1] << " " << n[2] << endl;
		}
		//		cout << q << " " << p << endl;
		//		cout << q << endl;
		//		cout << n << " " << l << endl;
		//		cout << x << endl 
		*/

    lightFacing[i] = n * l >= 0;

		//		cout << lightFacing[i] << " ";

  }
	//	cout << endl;

}



void SceneBoxVis::renderLightCap() {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glPushMatrix();

  // Get the transform
  btTransform trans;
  box->getMotionState()->getWorldTransform(trans);

  float m[16];
  trans.getOpenGLMatrix(m);

  glMultMatrixf(m);

  glBegin(GL_TRIANGLES);
  for (int i=0; i<mesh.getNumTri(); i++) {

    if (!lightFacing[i])
      continue;

    glVertex3fv(mesh.getVerts()[mesh.getIndices()[i*3  ]].get_ptr());
    glVertex3fv(mesh.getVerts()[mesh.getIndices()[i*3+1]].get_ptr());
    glVertex3fv(mesh.getVerts()[mesh.getIndices()[i*3+2]].get_ptr());

  }
  glEnd();

  glPopMatrix();

  glPopAttrib();

}

void SceneBoxVis::renderDarkCap(const Vector4f &lightPosition) {

  // Infinite lights don't have dark caps
  if (lightPosition[3] == 0.0)
    return;

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glPushMatrix();

  // Get the transform
  btTransform transform;
  box->getMotionState()->getWorldTransform(transform);

  Quaternionf q(transform.getRotation());
  Vector3f p(transform.getOrigin());


  glBegin(GL_TRIANGLES);
  for (int i=0; i<mesh.getNumTri(); i++) {

    if (lightFacing[i])
      continue;

    Vector3f v0 = q * mesh.getVerts()[mesh.getIndices()[i*3  ]] + p;
    Vector3f v1 = q * mesh.getVerts()[mesh.getIndices()[i*3+1]] + p;
    Vector3f v2 = q * mesh.getVerts()[mesh.getIndices()[i*3+2]] + p;

    Vector4f vFinal0 = Vector4f(v0[0], v0[1], v0[2], 0.0f) - lightPosition;
    Vector4f vFinal1 = Vector4f(v1[0], v1[1], v1[2], 0.0f) - lightPosition;
    Vector4f vFinal2 = Vector4f(v2[0], v2[1], v2[2], 0.0f) - lightPosition;

    vFinal0[3] = 0.0f;
    vFinal1[3] = 0.0f;
    vFinal2[3] = 0.0f;

    glVertex4fv(vFinal0.get_ptr());
    glVertex4fv(vFinal1.get_ptr());
    glVertex4fv(vFinal2.get_ptr());

  }
  glEnd();

  glPopMatrix();

  glPopAttrib();

}


void SceneBoxVis::renderShadowSides(const Vector4f &lightPosition) {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glPushMatrix();

	//	cout << endl << endl << endl;

	//	for (int i=0; i<mesh.getNumVerts(); i++)
	//		cout << mesh.getVerts()[i] << endl;

  // Get the transform
  btTransform transform;
  box->getMotionState()->getWorldTransform(transform);

  Quaternionf q(transform.getRotation());
  Vector3f p(transform.getOrigin());

  if (lightPosition[3] == 0.0f) {

    glBegin(GL_TRIANGLES);

    for (int i=0; i<mesh.getNumEdgePairs(); i++) {
      const Edge &edge0 = mesh.getEdges()[mesh.getEdgePairs()[i].e0];
      const Edge &edge1 = mesh.getEdges()[mesh.getEdgePairs()[i].e1];

      if (lightFacing[edge0.f] == lightFacing[edge1.f])
				continue;
			
      Vector3f v0;
      Vector3f v1;
      Vector4f v2 = -lightPosition;

      // the first face is facing the light
      if (lightFacing[edge0.f]) {

				v0 = q * mesh.getVerts()[edge0.v1] + p;
				v1 = q * mesh.getVerts()[edge0.v0] + p;

      } else {

				v0 = q * mesh.getVerts()[edge0.v0] + p;
				v1 = q * mesh.getVerts()[edge0.v1] + p;

      }

			/*
			cout << edge0.v0 << " " << edge0.v1 << endl;
			cout << q << endl;
			cout << mesh.getVerts()[edge0.v0] << endl;
			cout << mesh.getVerts()[edge0.v1] << endl;
			cout << p << endl;
			cout << v0 << endl;
			cout << v1 << endl;
			cout << v2 << endl;
			cout << endl;
			*/

      glVertex3fv(v0.get_ptr());
      glVertex3fv(v1.get_ptr());
      glVertex4fv(v2.get_ptr());

    }

    glEnd();

  } else {

    glBegin(GL_QUADS);

    for (int i=0; i<mesh.getNumEdgePairs(); i++) {
      const Edge &edge0 = mesh.getEdges()[mesh.getEdgePairs()[i].e0];
      const Edge &edge1 = mesh.getEdges()[mesh.getEdgePairs()[i].e1];

      if (lightFacing[edge0.f] == lightFacing[edge1.f])
				continue;

      Vector3f v0;
      Vector3f v1;
      Vector4f v2;
      Vector4f v3;

      // the first face is facing the light
      if (lightFacing[edge0.f]) {

				v0 = q * mesh.getVerts()[edge0.v1] + p;
				v1 = q * mesh.getVerts()[edge0.v0] + p;
				for (int j=0; j<3; j++) {
					v2[j] = v1[j] - lightPosition[j];
					v3[j] = v0[j] - lightPosition[j];
				}
				v2[3] = v3[3] = 0.0f;

      } else {

				v0 = q * mesh.getVerts()[edge0.v0] + p;
				v1 = q * mesh.getVerts()[edge0.v1] + p;
				for (int j=0; j<3; j++) {
					v2[j] = v1[j] - lightPosition[j];
					v3[j] = v0[j] - lightPosition[j];
				}
				v2[3] = v3[3] = 0.0f;

      }

      glVertex3fv(v0.get_ptr());
      glVertex3fv(v1.get_ptr());
      glVertex4fv(v2.get_ptr());
      glVertex4fv(v3.get_ptr());

    }

    glEnd();

  }

  glPopMatrix();

  glPopAttrib();


	//	mesh.dump();

}
