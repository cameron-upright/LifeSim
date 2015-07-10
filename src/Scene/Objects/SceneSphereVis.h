#ifndef SCENE_SPHERE_VIS
#define SCENE_SPHERE_VIS

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include "SceneSphere.h"

#include <math.h>
#include <iostream>

using namespace std;

class SceneSphereVis {

 private:
  SceneSphere *sphere;

 public:

  SceneSphereVis(SceneSphere *_sphere) : sphere(_sphere) {
  }

  void render(bool ambientPass) {

    glPushMatrix();

    // Get the transform
    btTransform trans;
    sphere->getMotionState()->getWorldTransform(trans);

    float m[16];

    trans.getOpenGLMatrix(m);

    glMultMatrixf(m);
    glutSolidSphere(sphere->radius, 16, 8);

    glPopMatrix();

  }

};

#endif
