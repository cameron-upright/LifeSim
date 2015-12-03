#ifndef CREATURE_ENV_H
#define CREATURE_ENV_H

#include <memory>
#include <mutex>
#include <condition_variable>

#include "Scene.h"

#include "RLGlue/RLGlue++.h"
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

	// The path of the scene configuration file (TODO : gross, used for restarting)
	string scenePath_;
	LifeSim::CreatureEnvDesc desc_;




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

	//  void start(RLGlue::StateDesc &state);

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


	std::unique_lock<std::mutex> getSimLock() {
		return std::unique_lock<std::mutex>(simMutex);
	}


	// RLGlue overrides

	RLGlue::StateDesc start() override;

	RLGlue::RewardStateTerminal step(const RLGlue::ActionDesc &action) override;





private:

	void updateCurrentState();

  void applyControl(const RLGlue::ActionDesc &action);

};




#endif // CREATURE_ENV_H
