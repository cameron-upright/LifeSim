#include "Experiment.h"


namespace RLGlue {

	void Experiment::runEpisode(const int stepLimit) {

		StateDesc state = envClient_.start();

		// Step the environment for 20 steps
		for (int i=0; i<20; i++) {

			RLGlue::RewardStateTerminal rewardStateTerminal = envClient_.step();

		}

	}

}
