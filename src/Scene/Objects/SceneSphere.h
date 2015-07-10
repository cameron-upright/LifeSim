#ifndef SCENE_SPHERE_H
#define SCENE_SPHERE_H

#include <btBulletDynamicsCommon.h>

#include "SceneRigidBodyObject.h"

#include "Vector3.h"


class SceneSphere : public SceneRigidBodyObject {

  // Bullet stuff
  btCollisionShape *sphereShape;
  btDefaultMotionState* sphereMotionState;
  btRigidBody* sphereRigidBody;

public:

  float radius;
  Vector3f origin;

  SceneSphere(string &name_, float radius_, Vector3f &origin_);
  ~SceneSphere();

  btCollisionShape* getCollisionShape() {
    return sphereShape;
  };

  btDefaultMotionState* getMotionState() {
    return sphereMotionState;
  }

  btRigidBody* getRigidBody() {
    return sphereRigidBody;
  }

};


#endif //SCENE_SPHERE_H
