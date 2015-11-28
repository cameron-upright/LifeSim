#ifndef SCENE_RIGID_BODY_OBJECT_H
#define SCENE_RIGID_BODY_OBJECT_H

#include <string>

#include "SceneObject.h"

#include "Transform.h"

using namespace std;

class SceneRigidBodyObject : public SceneObject {

public:

  SceneRigidBodyObject(string name_) : SceneObject(name_) {}
	virtual ~SceneRigidBodyObject() {}

	virtual void reset(const Transform &transform_) {}

	Vector3f getCenterOfMass() {
		return Vector3f(getRigidBody()->getCenterOfMassPosition());
	}

	Vector3f getVelocity() {
		return Vector3f(getRigidBody()->getLinearVelocity());
	}

	float getMass() {
		return 1.0 / getRigidBody()->getInvMass();
	}

  virtual btCollisionShape *getCollisionShape() = 0;
  virtual btDefaultMotionState* getMotionState() = 0;
  virtual btRigidBody* getRigidBody() = 0;

};


#endif // SCENE_RIGID_BODY_OBJECT_H
