#include "SceneSphere.h"



SceneSphere::SceneSphere(string &name_, float radius_, Vector3f &origin_) : SceneRigidBodyObject(name_), radius(radius_), origin(origin_) {

  sphereShape = new btSphereShape(radius_);

  sphereMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(origin[0],origin[1],origin[2])));
  btScalar mass = 4.0f/3.0f * M_PI * radius * radius * radius;
  btVector3 sphereInertia(0,0,0);
  sphereShape->calculateLocalInertia(mass,sphereInertia);
  btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass,sphereMotionState,sphereShape,sphereInertia);
  sphereRigidBodyCI.m_restitution = 0.5;
  sphereRigidBody = new btRigidBody(sphereRigidBodyCI);

}



SceneSphere::~SceneSphere() {

  delete sphereRigidBody;
  delete sphereMotionState;
  delete sphereShape;

}
