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

#include "SceneVis.h"

#include "g_axis.h"
#include "Vector4.h"

#include "Mesh.h"

#include <iostream>

using namespace std;

SceneVis::SceneVis(Scene &scene) : scene(scene){

  lookat = Vector3f(0.0, 0.5, 0.0);
  phi = -M_PI/8.0;
  theta = M_PI/4.0;
  dist = 8.0;
}

SceneVis::~SceneVis() {

	for (vector<SceneLight*>::iterator it = lights.begin(); it != lights.end(); it++)
		delete *it;

	for (vector<ScenePlaneVis*>::iterator it = planes.begin(); it != planes.end(); it++)
		delete *it;

	for (vector<SceneSphereVis*>::iterator it = spheres.begin(); it != spheres.end(); it++)
		delete *it;

	for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
		delete *it;

	for (vector<CreatureVis*>::iterator it = creatures.begin(); it != creatures.end(); it++)
		delete *it;

}

void SceneVis::onSceneAddLight(const Vector4f &pos_,
															 const Vector4f &diffuse_,
															 const Vector4f &specular_,
															 const Vector4f &ambient_) {

	lights.push_back(new SceneLight(pos_, diffuse_, specular_, ambient_));

}

void SceneVis::onSceneAddPlane(ScenePlane *plane, const Vector3f &origin, const float size) {
  planes.push_back(new ScenePlaneVis(plane, size, origin));
}

void SceneVis::onSceneAddSphere(SceneSphere *sphere) {
  spheres.push_back(new SceneSphereVis(sphere));
}

void SceneVis::onSceneAddBox(SceneBox *box) {
	boxes.push_back(new SceneBoxVis(box));
}

void SceneVis::onSceneAddCreature(Creature *creature) {
	creatures.push_back(new CreatureVis(creature));
}


void SceneVis::applyCameraTransform() {
  glTranslated(0,0,-dist);
  glRotated(-phi*180.0/M_PI, 1.0, 0.0, 0.0);
  glRotated(-theta*180.0/M_PI, 0.0, 1.0, 0.0);
  glTranslated(-lookat[0], -lookat[1], -lookat[2]);
}

void SceneVis::applyFog() {

	Vector4f black(0.0f, 0.0f, 0.0f, 0.0f);

  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_EXP);
  glFogf(GL_FOG_DENSITY, 0.003f);
  glFogfv(GL_FOG_COLOR, black.ptr());

}

void SceneVis::render(void) {


	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();

	applyCameraTransform();
	//	applyFog();

  glEnable(GL_CULL_FACE);

	glDisable(GL_FOG);


	renderAmbientPass();

	for (unsigned int i=0; i<lights.size(); i++) {
		renderStencilPass(i);
		renderPhongPass(i);
	}



	// Draw an axis at the origin
	glDisable(GL_CULL_FACE);
  glPushMatrix();
	//	glScaled(0.5, 0.5, 0.5);
  //	draw_axis();
  glPopMatrix();


  drawOrientationAxis(theta, phi);


}



void SceneVis::renderAmbientPass(void) {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

	float ambient[] = {0.8,0.8,0.8,1};
	float black[] = {0,0,0,1};
	float pos[] = {1,0,0,0};

	//	glColorMask(0,0,0,0);

	// We use lighting here to get an ambient contribution, through the material
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glColor4f(1,1,1,1);

	for (vector<SceneSphereVis*>::iterator it = spheres.begin(); it != spheres.end(); it++)
		(*it)->render(true);
	for (vector<CreatureVis*>::iterator it = creatures.begin(); it != creatures.end(); it++)
		(*it)->render(true);
  for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
		(*it)->render(true);
	for (vector<ScenePlaneVis*>::iterator it = planes.begin(); it != planes.end(); it++)
		(*it)->render(true);

	glColorMask(~0,~0,~0,~0);


  glPopAttrib();

}


void SceneVis::renderStencilPass(int lightIndex) {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Write to the stencil buffer, and clear it
  glStencilMask(~0);
  glClear(GL_STENCIL_BUFFER_BIT);

	// Test depth, but don't update
  glEnable(GL_DEPTH_TEST);
  glDepthMask(0);
	glDepthFunc(GL_LESS);

	// Don't write to color buffer, and don't use lighting
	glColorMask(0,0,0,0);
  glDisable(GL_LIGHTING);

  // enable two sided stenciling, with depth clamp
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
  glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_CLAMP_NV);

  // setup front and back stencil ops
  glActiveStencilFaceEXT(GL_FRONT);
  glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);
  glStencilMask(~0);
  glStencilFunc(GL_ALWAYS, 0, ~0);

  glActiveStencilFaceEXT(GL_BACK);
  glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
  glStencilMask(~0);
  glStencilFunc(GL_ALWAYS, 0, ~0);


	/*
	//	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthFunc(0);
	glColor4f(0,1,0,0.1);
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_CLAMP_NV);

	glDepthFunc(GL_LEQUAL);
	*/

	for (vector<SceneSphereVis*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
		//		(*it)->calcLightFacing(lights[lightIndex]->pos);
		//		(*it)->renderLightCap();
		//		(*it)->renderShadowSides(lights[lightIndex]->pos);
	}

	for (vector<CreatureVis*>::iterator it = creatures.begin(); it != creatures.end(); it++) {
		(*it)->calcLightFacing(lights[lightIndex]->pos);
		(*it)->renderLightCap();
		(*it)->renderDarkCap(lights[lightIndex]->pos);
		(*it)->renderShadowSides(lights[lightIndex]->pos);
	}

	//	glEnable(GL_POLYGON_OFFSET_FILL);
	//	glPolygonOffset(0.0, 10.5);

  for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
		(*it)->calcLightFacing(lights[lightIndex]->pos);
		(*it)->renderLightCap();
		(*it)->renderDarkCap(lights[lightIndex]->pos);
		(*it)->renderShadowSides(lights[lightIndex]->pos);
	}

	//	glDisable(GL_POLYGON_OFFSET_FILL);

  glPopAttrib();

}


void SceneVis::renderPhongPass(int lightIndex) {

  glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Turn on lighting
	glEnable(GL_LIGHTING);

	// Don't write to depth buffer, and test for equality, since the ambient pass already happened
	glDepthFunc(GL_LEQUAL);
	glDepthMask(0);

	// Write to color buffer
  glColorMask(~0,~0,~0,~0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);


	// Use stencils, don't update, check for = 0
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0);
	glStencilFunc(GL_EQUAL, 0, ~0);

  glEnable(GL_CULL_FACE);

	// Setup this light
	lights[lightIndex]->applyLight(0);

	for (vector<SceneSphereVis*>::iterator it = spheres.begin(); it != spheres.end(); it++)
		(*it)->render(false);
	for (vector<CreatureVis*>::iterator it = creatures.begin(); it != creatures.end(); it++)
		(*it)->render(false);
  for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
		(*it)->render(false);
	for (vector<ScenePlaneVis*>::iterator it = planes.begin(); it != planes.end(); it++)
		(*it)->render(false);


	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPopAttrib();

}
