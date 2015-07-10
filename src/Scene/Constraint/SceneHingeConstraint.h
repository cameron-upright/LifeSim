#ifndef SCENE_HINGE_CONSTRAINT_H
#define SCENE_HINGE_CONSTRAINT_H

#include <string>

#include "SceneConstraint.h"
#include "Vector2.h"

using namespace std;

class SceneHingeConstraint : public SceneConstraint {

  btVector3 axisA, axisB;
  btVector3 pivotA, pivotB;

  Vector2f limit;

  SceneRigidBodyObject *rigidBodyA, *rigidBodyB;

  btHingeConstraint *hingeConstraint;

  // Bullet seems to consider zero angle as the angle where both frames of reference are aligned
  // This is the original angle according to Bullet, when we setup the constraint
  float zeroAngle; 

public:

  SceneHingeConstraint(const btVector3 &axisA_, const btVector3 &axisB_,
											 const btVector3 &pivotA_, const btVector3 &pivotB_,
											 const Vector2f &limit_,
											 SceneRigidBodyObject *rigidBodyA_, SceneRigidBodyObject *rigidBodyB_,
											 const string &name) : SceneConstraint(name),
																						 axisA(axisA_), axisB(axisB_),
																						 pivotA(pivotA_), pivotB(pivotB_) {

    rigidBodyA = rigidBodyA_;
    rigidBodyB = rigidBodyB_;

    limit = limit_;

    hingeConstraint = new btHingeConstraint(*rigidBodyA->getRigidBody(), *rigidBodyB->getRigidBody(), pivotA, pivotB, axisA, axisB);

    for (int i=0; i<6; i++) {
      hingeConstraint->setParam(BT_CONSTRAINT_STOP_ERP, 0.2, i);
      hingeConstraint->setParam(BT_CONSTRAINT_STOP_CFM, 0.2, i);
    }

    zeroAngle = hingeConstraint->getHingeAngle();

    hingeConstraint->setLimit(zeroAngle+limit[0], zeroAngle+limit[1]);
    //    hingeConstraint->enableAngularMotor(true, 0, 1);

  }

  ~SceneHingeConstraint() {
    delete hingeConstraint;
  }

  btTypedConstraint *getConstraint() {
    return hingeConstraint;
  }

  SceneConstraintType getConstraintType() {
    return SceneHingeConstraintType;
  }

  float getAngle() {
    return hingeConstraint->getHingeAngle() - zeroAngle;
  }

  void enableAngularMotor(bool enabled, float targetVelocity, float maxMotorImpulse) {
    hingeConstraint->enableAngularMotor(enabled, targetVelocity, maxMotorImpulse);
  }


};


#endif // SCENE_HINGE_CONSTRAINT_H
