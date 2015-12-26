#ifndef CREATURE_H
#define CREATURE_H

#include "ScenePhysics.h"

#include <string>
#include <vector>
#include <ext/hash_map>
#include <memory>

#include "SceneObject.h"
#include "SceneBox.h"

#include "SceneHingeConstraint.h"
#include "SceneUniversalConstraint.h"

using namespace std;

struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

class Scene;

class Creature : public SceneObject {

public:

	LifeSim::SceneCreatureDesc creatureDesc;

	vector<shared_ptr<SceneHingeConstraint> > hingeConstraints;
	vector<shared_ptr<SceneUniversalConstraint> > universalConstraints;

  map<string, shared_ptr<SceneConstraint> > constraintMap;
  map<string, shared_ptr<SceneRigidBodyObject> > rigidBodyMap;

	map<pair<string,int>, float> symmetryMultiplier;

	map<string, vector<string> > groupBodyMap;
	map<string, vector<string> > groupConstraintMap;

	// TODO : gross
	// This map stores the original rotations of the boxes
	map<string,btQuaternion> originalRotation;


	Creature() {}
	Creature(const Creature &creature) = delete;
	Creature(Creature &&creature) = delete;


	Creature(const string &name_) : SceneObject(name_) {}

  ~Creature();

	Creature& operator=(const Creature &other) = delete;
	Creature& operator=(Creature &&other) = delete;

  bool load(const string &filename);

  void addToScenePhysics(ScenePhysics *scenePhysics);
  void removeFromScenePhysics(ScenePhysics *scenePhysics);

	Vector3f getVelocity() const {
		Vector3f velocity;
		float totalMass = 0.0f;

		for (auto &it : rigidBodyMap) {
			float mass = it.second->getMass();
			totalMass += mass;
			velocity += it.second->getVelocity() * mass;
		}

		return velocity / totalMass;
	}

	Vector3f getCenterOfMass() const {

		Vector3f com;
		float totalMass = 0.0f;

		for (auto &it : rigidBodyMap) {
			float mass = it.second->getMass();
			totalMass += mass;
			com += it.second->getCenterOfMass() * mass;
		}

		return com / totalMass;

	}


	Vector3f getUpDirection() const {

		Vector3f up;
		float totalMass = 0.0f;

		for (auto &it : rigidBodyMap) {

			SceneBox *box = dynamic_cast<SceneBox*>(it.second.get());

			if (box == NULL)
				continue;

			btTransform currentTransform;
			btQuaternion startRotation = (originalRotation.find(box->getName()))->second;

			box->getMotionState()->getWorldTransform(currentTransform);
			currentTransform.setRotation(currentTransform.getRotation() * startRotation.inverse());

			float mass = box->getMass();
			totalMass += mass;

			up += Quaternionf(currentTransform.getRotation()) * Vector3f(0,1,0) * mass;

		}

		return up / totalMass;

	}


	int getNumDOF() const {
		return hingeConstraints.size() + universalConstraints.size() * 2;
	}


};


#endif
