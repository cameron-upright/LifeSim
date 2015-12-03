#ifndef SCENE_UNIVERSAL_CONSTRAINT_H
#define SCENE_UNIVERSAL_CONSTRAINT_H

#include <string>

#include "SceneConstraint.h"

#include "Vector2.h"

using namespace std;

class SceneUniversalConstraint : public SceneConstraint{

  btVector3 axis0, axis1;
  btVector3 pivot;

  Vector2f limit0, limit1;

  SceneRigidBodyObject *rigidBodyA, *rigidBodyB;

  btUniversalConstraint *universalConstraint;

public:

  SceneUniversalConstraint(const btVector3 &axis0_, const btVector3 &axis1_,
													 const btVector3 &pivot_, const Vector2f &limit0_, const Vector2f &limit1_,
													 SceneRigidBodyObject *rigidBodyA_, SceneRigidBodyObject *rigidBodyB_,
													 const string &name) : SceneConstraint(name), axis0(axis0_), axis1(axis1_), pivot(pivot_), limit0(limit0_), limit1(limit1_) {
    
    rigidBodyA = rigidBodyA_;
    rigidBodyB = rigidBodyB_;

    universalConstraint = new btUniversalConstraint(*rigidBodyA->getRigidBody(), *rigidBodyB->getRigidBody(), pivot, axis1, axis0);

    for (int i=0; i<6; i++) {
      universalConstraint->setParam(BT_CONSTRAINT_STOP_ERP, 0.2, i);
      universalConstraint->setParam(BT_CONSTRAINT_STOP_CFM, 0.2, i);
    }

    /*
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 0) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 1) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 2) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 3) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 4) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_ERP, 5) << endl;

    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 0) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 1) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 2) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 3) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 4) << endl;
    cout << universalConstraint->getParam(BT_CONSTRAINT_STOP_CFM, 5) << endl;
    */

    universalConstraint->setLowerLimit(limit0[0], limit1[0]);
    universalConstraint->setUpperLimit(limit0[1], limit1[1]);

    /*
    cout << universalConstraint->getRotationalLimitMotor(2)->m_maxLimitForce << endl;
    cout << universalConstraint->getRotationalLimitMotor(2)->m_maxMotorForce << endl;

    universalConstraint->getRotationalLimitMotor(1)->m_enableMotor = true;
    universalConstraint->getRotationalLimitMotor(1)->m_targetVelocity = 0.0;
    //    universalConstraint->getRotationalLimitMotor(1)->m_maxMotorForce = 100000;
    //    universalConstraint->getRotationalLimitMotor(1)->m_maxLimitForce = 100000;
    universalConstraint->getRotationalLimitMotor(2)->m_enableMotor = true;
    universalConstraint->getRotationalLimitMotor(2)->m_targetVelocity = 0.0;
    //    universalConstraint->getRotationalLimitMotor(2)->m_maxMotorForce = 100000;
    //    universalConstraint->getRotationalLimitMotor(2)->m_maxLimitForce = 100000;
    */

  }

  ~SceneUniversalConstraint() {
    delete universalConstraint;
  }

  btTypedConstraint *getConstraint() {
    return universalConstraint;
  }

  SceneConstraintType getConstraintType() {
    return SceneUniversalConstraintType;
  }

  float getAngle(int index) const {
    return universalConstraint->getAngle(index+1);
  }

  void enableAngularMotor(int index, bool enabled, float targetVelocity, float maxMotorImpulse) {

    universalConstraint->getRotationalLimitMotor(index+1)->m_enableMotor = enabled;
    universalConstraint->getRotationalLimitMotor(index+1)->m_targetVelocity = targetVelocity;
    universalConstraint->getRotationalLimitMotor(index+1)->m_maxMotorForce = maxMotorImpulse;

  }


};


#endif // SCENE_UNIVERSAL_CONSTRAINT_H
