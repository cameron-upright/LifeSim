#ifndef RL_ACTION_H
#define RL_ACTION_H

#include <string>
#include <map>

using namespace std;

class RLConstraintAction {

public:

  float targetVelocity[2], maxMotorImpulse[2];

  RLConstraintAction() {}

  RLConstraintAction(const float targetVelocity0, const float targetVelocity1,
		     const float maxMotorImpulse0, const float maxMotorImpulse1) {

    targetVelocity[0] = targetVelocity0;
    targetVelocity[1] = targetVelocity1;

    maxMotorImpulse[0] = maxMotorImpulse0;
    maxMotorImpulse[1] = maxMotorImpulse1;

  }

};


class RLAction {

public:

  map<string, RLConstraintAction> constraintActions;

  const RLConstraintAction* getConstraintAction(const string name) const {
    map<string, RLConstraintAction>::const_iterator it = constraintActions.find(name);
    if (it != constraintActions.end())
      return &(it->second);
    else
      return NULL;
  }

};


#endif // RL_ACTION_H
