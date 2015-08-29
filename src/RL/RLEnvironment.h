#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include <mutex>
#include <condition_variable>

#include "Scene.h"

#include "LifeSim.pb.h"


class RLEnvironment {

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
  unique_ptr<LifeSim::RLStateDesc>  currentState;
  unique_ptr<LifeSim::RLActionDesc> currentAction;

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

  RLEnvironment();
  ~RLEnvironment();

  void load(const string &filename);

  void start(LifeSim::RLStateDesc &state);

	void stepSim(const float dt);
  void stepRL(LifeSim::RLStateDesc &state, const LifeSim::RLActionDesc &action, float &reward);

  Scene* getScene() {
    return scene;
  }

	Creature* getCreature() {
		return creature;
	}

	float getEnvStepSize() const {
		return envStepSize;
	}

private:

  void applyControl(const LifeSim::RLActionDesc &action);

};




#endif // RL_ENVIRONMENT_H
