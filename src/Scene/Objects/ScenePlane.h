#ifndef SCENE_PLANE_H
#define SCENE_PLANE_H

#include <btBulletDynamicsCommon.h>

#include "SceneRigidBodyObject.h"
#include "Plane.h"

class ScenePlane : public SceneRigidBodyObject {

  // Bullet stuff
  btCollisionShape *planeShape;
  btDefaultMotionState* planeMotionState;
  btRigidBody* planeRigidBody;

public:

  // plane object
  Planef plane;

  ScenePlane(const string &name_, const Planef &plane_);
  ~ScenePlane();

  btCollisionShape* getCollisionShape() {
    return planeShape;
  };

  btDefaultMotionState* getMotionState() {
    return planeMotionState;
  }

  btRigidBody* getRigidBody() {
    return planeRigidBody;
  }


};


#endif //SCENE_PLANE_H
