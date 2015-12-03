#include "ScenePlane.h"



ScenePlane::ScenePlane(const string &name_, const Planef &plane_) : SceneRigidBodyObject(name_), plane(plane_) {

  planeShape = new btStaticPlaneShape(btVector3(plane.v[0], plane.v[1], plane.v[2]), -plane.d);
  planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));

  btRigidBody::btRigidBodyConstructionInfo
    planeRigidBodyCI(0, planeMotionState, planeShape, btVector3(0,0,0));
  planeRigidBodyCI.m_restitution = 0.5;
  //  planeRigidBodyCI.m_friction = 2.0;
  planeRigidBody = new btRigidBody(planeRigidBodyCI);

}



ScenePlane::~ScenePlane() {

  delete planeRigidBody;
  delete planeMotionState;
  delete planeShape;

}
