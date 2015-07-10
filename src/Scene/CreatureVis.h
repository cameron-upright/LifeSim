#ifndef CREATURE_VIS_H
#define CREATURE_VIS_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include "Creature.h"
#include "SceneBoxVis.h"

#include <math.h>
#include <iostream>

using namespace std;

class CreatureVis : public SceneObjectVis {

 private:
  Creature *creature;
  vector<SceneBoxVis*> boxes;

 public:

  CreatureVis(Creature *_creature) : creature(_creature) {
    for (vector<SceneBox*>::iterator it = creature->boxes.begin(); it != creature->boxes.end(); it++)
      boxes.push_back(new SceneBoxVis(*it));
  }

  ~CreatureVis() {
    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      delete *it;
  }

  void render(bool ambientPass) {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->render(ambientPass);

  }

  void renderLightCap() {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->renderLightCap();

  }

  void renderDarkCap(const Vector4f &lightPosition) {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->renderDarkCap(lightPosition);

  }


  void renderShadowSides(const Vector4f &lightPosition) {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->renderShadowSides(lightPosition);

  }

  void calcLightFacing(const Vector4f &lightPosition) {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->calcLightFacing(lightPosition);

  }


};

#endif
