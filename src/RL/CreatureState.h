#ifndef CREATURE_STATE_H
#define CREATURE_STATE_H

#include "RLGlue/RLGlue++.h"
#include "RLGlue.pb.h"

#include "Proto/LifeSim.pb.h"

class CreatureState {

public:

	CreatureState() {}
  CreatureState(const RLGlue::StateDesc &stateDesc, const LifeSim::SceneCreatureDesc &sceneCreatureDesc);


	operator RLGlue::StateDesc() const {

		RLGlue::StateDesc stateDesc;

		for (const auto &kv : constraintAngles)
			for (const auto &angle : kv.second)
				stateDesc.add_float_state(angle);

		return stateDesc;

	}

	void logStateInfo() const {

		for (const auto &kv : constraintAngles)
			for (const auto &val : kv.second)
				LOG(INFO) << kv.first << " : " << val;
		LOG(INFO);

	}

	// For each constraint, a vector of angles
	std::map<std::string, vector<float> > constraintAngles;

};


#endif // CREATURE_STATE_H
