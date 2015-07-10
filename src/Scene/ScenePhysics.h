#ifndef SCENE_PHYSICS_H
#define SCENE_PHYSICS_H

#include <istream>

#include <btBulletDynamicsCommon.h>

#include "SceneRigidBodyObject.h"
#include "SceneConstraint.h"

using namespace std;

class ScenePhysics {

  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;

  btSequentialImpulseConstraintSolver* solver;

  btDiscreteDynamicsWorld* dynamicsWorld;

public:

  ScenePhysics(void);
  ~ScenePhysics();

  void addSceneRigidBody(SceneRigidBodyObject *object);
  void removeSceneRigidBody(SceneRigidBodyObject *object);

  void addSceneConstraint(SceneConstraint *constraint);
  void removeSceneConstraint(SceneConstraint *constraint);

  void step(const float step);

};

#endif
