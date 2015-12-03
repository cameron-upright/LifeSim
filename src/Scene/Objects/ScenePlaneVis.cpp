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


#include "ScenePlaneVis.h"



void ScenePlaneVis::render(bool ambientPass) {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glPushMatrix();

  applyTransform();

  glEnable(GL_NORMALIZE);

  material->applyMaterial();


	// Use the vertex program and fragment program
	if (vertexProgram && fragmentProgram && !ambientPass) {

		glEnable(GL_VERTEX_PROGRAM_ARB);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgram->vertexProgram);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragmentProgram->fragmentProgram);

	}

	glScaled(scale, scale, scale);

  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0); glVertex3f(-1.0, -0.0001,  1.0);
  glNormal3f(0, 1, 0); glVertex3f( 1.0, -0.0001,  1.0);
  glNormal3f(0, 1, 0); glVertex3f( 1.0, -0.0001, -1.0);
  glNormal3f(0, 1, 0); glVertex3f(-1.0, -0.0001, -1.0);
  glEnd();


	if (vertexProgram && fragmentProgram) {

		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, 0);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, 0);

	}

  glPopMatrix();

  glPopAttrib();

}
