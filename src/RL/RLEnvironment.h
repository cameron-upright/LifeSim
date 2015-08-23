#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include <mutex>
#include <condition_variable>

#include "Scene.h"

#include "LifeSim.pb.h"


class RLEnvironment {

  Scene *scene;
  Creature *creature;

	float envStepSize;
	float envStepPerRLStep;

	// The leftover time that hasn't been simulated
	float remainingTime;

  // This increments every environment step, and wraps to 0 after numEnvStepsPerRLStep
  int envStep;

	// The most recent state, and the action we're currently simulating
  unique_ptr<LifeSim::RLStateDesc>  currentState;
  unique_ptr<LifeSim::RLActionDesc> currentAction;
	float currentReward;

	// A mutex for updating the state action and reward
	std::mutex mutex;

	// Used when the simulation is done, and stepRL can continue
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
