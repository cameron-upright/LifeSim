#ifndef SCENE_RIGID_BODY_OBJECT_H
#define SCENE_RIGID_BODY_OBJECT_H

#include <string>

#include "SceneObject.h"

using namespace std;

class SceneRigidBodyObject : public SceneObject {

public:

  SceneRigidBodyObject(string name_) : SceneObject(name_) {}

  virtual btCollisionShape *getCollisionShape() = 0;
  virtual btDefaultMotionState* getMotionState() = 0;
  virtual btRigidBody* getRigidBody() = 0;

};


#endif // SCENE_RIGID_BODY_OBJECT_H
