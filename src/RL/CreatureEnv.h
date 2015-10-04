#ifndef CREATURE_ENV_H
#define CREATURE_ENV_H

#include <memory>
#include <mutex>
#include <condition_variable>

#include "Scene.h"

#include "RLGlue++.h"
#include "RLGlue.pb.h"


class CreatureEnv : public RLGlue::Env {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Simulation
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// The state of the simulation
  Scene *scene;
  Creature *creature;

	// The leftover time that hasn't been simulated
	float remainingTime;

	// The discrete step size of the simulation
	float envStepSize;

	// The number of environment steps before we require a new action
	float envStepPerRLStep;

  // This increments every environment step, and wraps to 0 after numEnvStepsPerRLStep
  int envStep;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// RL Info
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// The most recent state, and the action we're currently simulating
	std::unique_ptr<RLGlue::StateDesc>  currentState;
  std::unique_ptr<RLGlue::ActionDesc> currentAction;

	// The reward
	float currentReward;


	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mutex / Condition
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// Locks the simulation variables
	std::mutex simMutex;

	// Locks the RL variables
	std::mutex rlMutex;

	// Used to notify env_step that the simulation is complete
	std::mutex stepMutex;
	std::condition_variable stepCond;



public:

  CreatureEnv();
  ~CreatureEnv();

  void load(const string &filename);

  void start(RLGlue::StateDesc &state);

	void stepSim(const float dt);
  void stepRL(RLGlue::StateDesc &state, const RLGlue::ActionDesc &action, float &reward);

  Scene* getScene() {
    return scene;
  }

	Creature* getCreature() {
		return creature;
	}

	float getEnvStepSize() const {
		return envStepSize;
	}



	// RLGlue overrides

	RLGlue::StateDesc start() override {
	}

	RLGlue::RewardStateTerminal step() override {

		const float constraintMultiplier = 0.5f;


		RLGlue::ActionDesc action;

		static vector<float> actionVal(getCreature()->hingeConstraints.size() + getCreature()->universalConstraints.size()*2);


		int actionIndex = 0;
		for (auto &a : actionVal) {
			a *= 0.95;
			if (lrand48() % 4 == 0)
				a += 15.0*(drand48()-0.5);
			action.add_float_action(-constraintMultiplier * a);
		}


		// Prepare the step, creating an action to resist movement
		RLGlue::StateDesc state;
		float the_reward=0;

		// Step the environment
		stepRL(state, action, the_reward);


		RLGlue::RewardStateTerminal rst;

		rst.set_reward(0.0);
		rst.mutable_state();
		rst.set_terminal(false);

		return rst;

	}






private:

  void applyControl(const RLGlue::ActionDesc &action);

};




#endif // CREATURE_ENV_H
