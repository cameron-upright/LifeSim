#include <glog/logging.h>
#include "CreatureAction.h"

CreatureAction::CreatureAction(const RLGlue::ActionDesc &actionDesc, const LifeSim::SceneCreatureDesc &sceneCreatureDesc) {

	// A CreatureAction and therefore an ActionDesc stores things in alphabetical order.  We need to find this order in order to recreate this
	// from the ActionDesc.  Create the constraintActivations map with appropriately sized vectors, then iterate through that map,
	// populating the vectors with data from the actionDesc.
	// TODO : This kind of sucks, profile and find a better way perhaps (PT https://www.pivotaltracker.com/story/show/110791096)
	for (int i=0; i<sceneCreatureDesc.constraint_size(); i++) {

		const LifeSim::ConstraintDesc &constraintDesc = sceneCreatureDesc.constraint(i);

		if (constraintDesc.type() == LifeSim::ConstraintDesc_Type_HINGE)
			constraintActivations[constraintDesc.name()] = vector<float>(1);
		else if (constraintDesc.type() == LifeSim::ConstraintDesc_Type_UNIVERSAL)
			constraintActivations[constraintDesc.name()] = vector<float>(2);

	}


	// The current float action we're reading
	int floatIndex = 0;

	for (auto &kv : constraintActivations) {

		for (int i=0; i<kv.second.size(); i++)
			kv.second[i] = actionDesc.float_action(floatIndex++);

	}

}
