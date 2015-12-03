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

	// TODO : gross
	// TODO : duplicated in Creature
	// This array stores the original rotations of the boxes
	vector<btQuaternion> originalRotation;

 public:

  CreatureVis(Creature *_creature) : creature(_creature) {

		for (auto &it : creature->rigidBodyMap) {
			SceneBox *box = dynamic_cast<SceneBox*>(it.second.get());
			if (box) {
				boxes.push_back(new SceneBoxVis(box));

				btTransform currentTransform;
				box->getMotionState()->getWorldTransform(currentTransform);
				originalRotation.push_back(currentTransform.getRotation());
			}
		}

  }

  ~CreatureVis() {
    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      delete *it;
  }

	Creature* getCreature() {
		return creature;
	}

  void render(bool ambientPass) {

    for (vector<SceneBoxVis*>::iterator it = boxes.begin(); it != boxes.end(); it++)
      (*it)->render(ambientPass);

  }

	void renderTransform() {

		for (unsigned i=0; i<boxes.size(); i++) {

			glPushMatrix();

			btTransform currentTransform;
			boxes[i]->getBox()->getMotionState()->getWorldTransform(currentTransform);
			currentTransform.setRotation(currentTransform.getRotation() * originalRotation[i].inverse());

			float m[16];
			currentTransform.getOpenGLMatrix(m);

			glMultMatrixf(m);

			/*
			  v   = R * v_0
				v   = R * (inv(R_0) * (0,1,0))
				v   = R * inv(R_0) * (0,1,0)
			*/

      boxes[i]->renderTransform();
			glPopMatrix();
		}

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
