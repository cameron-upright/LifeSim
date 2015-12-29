#include <glog/logging.h>
#include "CreatureState.h"

CreatureState::CreatureState(const RLGlue::StateDesc &stateDesc, const LifeSim::SceneCreatureDesc &sceneCreatureDesc) {

	// A CreatureState and therefore a StateDesc stores things in alphabetical order.  We need to find this order in order to recreate this
	// from the StateDesc.  Create the constraintAngles map with appropriately sized vectors, then iterate through that map,
	// populating the vectors with data from the stateDesc.
	// TODO : This kind of sucks, profile and find a better way perhaps (PT https://www.pivotaltracker.com/story/show/110791096)
	for (int i=0; i<sceneCreatureDesc.constraint_size(); i++) {

		const LifeSim::ConstraintDesc &constraintDesc = sceneCreatureDesc.constraint(i);

		if (constraintDesc.type() == LifeSim::ConstraintDesc_Type_HINGE)
			constraintAngles[constraintDesc.name()] = vector<float>(1);
		else if (constraintDesc.type() == LifeSim::ConstraintDesc_Type_UNIVERSAL)
			constraintAngles[constraintDesc.name()] = vector<float>(2);

	}


	// The current float state we're reading
	int floatIndex = 0;

	for (auto &kv : constraintAngles) {

		for (int i=0; i<kv.second.size(); i++)
			kv.second[i] = stateDesc.float_state(floatIndex++);

	}

}
