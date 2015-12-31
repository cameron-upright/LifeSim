#ifndef CREATURE_ACTION_H
#define CREATURE_ACTION_H

#include "RLGlue/RLGlue++.h"
#include "RLGlue.pb.h"

#include "Proto/LifeSim.pb.h"

class CreatureAction {

public:

	CreatureAction() {}
  CreatureAction(const RLGlue::ActionDesc &actionDesc, const LifeSim::SceneCreatureDesc &sceneCreatureDesc);


	operator RLGlue::ActionDesc() const {

		RLGlue::ActionDesc actionDesc;

		for (const auto &kv : constraintActivations)
			for (const auto &val : kv.second)
				actionDesc.add_float_action(val);

		return actionDesc;

	}

	void logActionInfo() const {

		for (const auto &kv : constraintActivations)
			for (const auto &val : kv.second)
				LOG(INFO) << kv.first << " : " << val;
		LOG(INFO);

	}

	// For each constraint, a vector activations
	std::map<std::string, vector<float> > constraintActivations;

};


#endif // CREATURE_ACTION_H
