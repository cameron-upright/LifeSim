#include "Experiment.h"


namespace RLGlue {

	void Experiment::runEpisode(const int stepLimit) {

		StateDesc state = envClient_.start();
		ActionDesc action = agentClient_.start();


		// Step the episode
		for (int i=0; i<stepLimit; i++) {

			RLGlue::RewardStateTerminal rewardStateTerminal = envClient_.step();

		}

		// End the episode
		//		agentClient_.end(0.0);

	}

}
