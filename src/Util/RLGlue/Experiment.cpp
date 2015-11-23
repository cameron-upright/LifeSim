#include "Experiment.h"


namespace RLGlue {

	void Experiment::runEpisode(const int stepLimit) {

		StateDesc state = envClient_.start();
		ActionDesc action = agentClient_.start(state);

		// Step the episode
		for (int i=0; i<stepLimit; i++) {

			// Step the environment
			RewardStateTerminal rewardStateTerminal = envClient_.step(action);

			// Create the RewardState
			RewardState rewardState;

			rewardState.set_reward(rewardStateTerminal.reward());
			*(rewardState.mutable_state()) = rewardStateTerminal.state();

			// Step the action
			action = agentClient_.step(rewardState);

		}

		// End the episode
		agentClient_.end(0.0);

	}

}

