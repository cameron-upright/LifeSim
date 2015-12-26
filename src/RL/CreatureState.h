#ifndef CREATURE_STATE_H
#define CREATURE_STATE_H

#include "RLGlue/RLGlue++.h"
#include "RLGlue.pb.h"

#include "Proto/LifeSim.pb.h"

class CreatureState {

public:

	operator RLGlue::StateDesc() const {

		RLGlue::StateDesc stateDesc;

		for (const auto &kv : groupAngles)
			for (const auto &angle : kv.second)
				stateDesc.add_float_state(angle);

		return stateDesc;

	}


	// For each group, this gives the concatenated angles of each joint
	std::map<std::string, vector<float> > groupAngles;

};


#endif // CREATURE_STATE_H
