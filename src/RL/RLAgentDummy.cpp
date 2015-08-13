#include <fstream>
#include <istream>
#include <sstream>

#include "RLAgentDummy.h"



void RLAgentDummy::step(const LifeSim::RLStateDesc &state, LifeSim::RLActionDesc &action) {

	const float constraintMultiplier = 0.0f;

	action.Clear();

	for (unsigned i=0; i<this->creature->hingeConstraints.size(); i++)
		action.add_action(-constraintMultiplier * this->creature->hingeConstraints[i]->getAngle());


	for (unsigned i=0; i<this->creature->universalConstraints.size(); i++) {
		action.add_action(-constraintMultiplier * this->creature->universalConstraints[i]->getAngle(0));
		action.add_action(-constraintMultiplier * this->creature->universalConstraints[i]->getAngle(1));
	}

	assert(action.action_size() == this->creature->hingeConstraints.size() + this->creature->universalConstraints.size() * 2);

}
