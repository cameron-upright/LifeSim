#include <btBulletDynamicsCommon.h>
#include <iostream>
#include "ScenePhysics.h"


ScenePhysics::ScenePhysics(void) {

  // Setup the broadphase, collision config and collision dispatcher
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);

  // Setup solver
  solver = new btSequentialImpulseConstraintSolver;

  // Setup the world
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
																							broadphase,
																							solver,
																							collisionConfiguration);

  // Gravity
  dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

  btContactSolverInfo solverInfo = dynamicsWorld->getSolverInfo();
  solverInfo.m_erp = 0.2;
  solverInfo.m_globalCfm = 0.2;

  //  cout << "ERP, CFM : " << solverInfo.m_erp << " " << solverInfo.m_globalCfm << endl;

}

ScenePhysics::~ScenePhysics() {

  // Clean up
  delete dynamicsWorld;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;

  
}


void ScenePhysics::reset() {

	solver->reset();
	dynamicsWorld->clearForces();
	broadphase->resetPool(dispatcher);

	btOverlappingPairCache *pairCache = broadphase->getOverlappingPairCache();
	btBroadphasePairArray &pairArray = pairCache->getOverlappingPairArray();

	for (int i=0; i<pairArray.size(); i++)
		pairCache->cleanOverlappingPair(pairArray[i], dispatcher);

}

void ScenePhysics::addSceneRigidBody(SceneRigidBodyObject *object) {

  dynamicsWorld->addRigidBody(object->getRigidBody());

}


void ScenePhysics::removeSceneRigidBody(SceneRigidBodyObject *object) {

  dynamicsWorld->removeRigidBody(object->getRigidBody());

}

void ScenePhysics::addSceneConstraint(SceneConstraint *constraint) {

  dynamicsWorld->addConstraint(constraint->getConstraint(), true);

}

void ScenePhysics::removeSceneConstraint(SceneConstraint *constraint) {

  dynamicsWorld->removeConstraint(constraint->getConstraint());

}


void ScenePhysics::step(const float step) {

  dynamicsWorld->stepSimulation(step, 100, 1.0f/400.0f);

}
