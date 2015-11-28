#include "SceneBox.h"

SceneBox::SceneBox(const string &name_,
									 const Vector3f &halfExtents_,
									 const Transform &transform_) :	SceneRigidBodyObject(name_), halfExtents(halfExtents_), transform(transform_) {

	create(halfExtents_, transform_);

}

void SceneBox::create(const Vector3f &halfExtents_,
											const Transform &transform_) {

  boxShape = new btBoxShape(btVector3(halfExtents[0], halfExtents[1], halfExtents[2]));

  boxMotionState =
    new btDefaultMotionState(transform);
  btScalar mass = halfExtents[0] * halfExtents[1] * halfExtents[2] * 8.0f;
  btVector3 boxInertia(0,0,0);
  boxShape->calculateLocalInertia(mass,boxInertia);
  btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass,boxMotionState,boxShape,boxInertia);
  boxRigidBodyCI.m_restitution = 0.5;
	boxRigidBodyCI.m_friction = 1.8;
  boxRigidBody = new btRigidBody(boxRigidBodyCI);

	//  btVector3 origin = boxRigidBody->getCenterOfMassTransform().getOrigin();

  //  boxRigidBody->setDamping(0.95, 0.95);

  /*
  cout << boxRigidBodyCI.m_friction << endl;
  cout << boxRigidBody->getLinearDamping() << endl;
  cout << boxRigidBody->getAngularDamping() << endl;
  */

}



void SceneBox::reset(const Vector3f &halfExtents_,
										 const Transform &transform_) {

	destroy();
	create(halfExtents_, transform_);

	/*
	boxMotionState->setWorldTransform(transform);
	boxRigidBody->setWorldTransform(transform);

	boxRigidBody->clearForces();
	boxRigidBody->setLinearVelocity(btVector3(0,0,0));
	boxRigidBody->setAngularVelocity(btVector3(0,0,0));
	*/

}



void SceneBox::destroy() {

  delete boxRigidBody;
  delete boxMotionState;
  delete boxShape;

}


SceneBox::~SceneBox() {

	destroy();

}

