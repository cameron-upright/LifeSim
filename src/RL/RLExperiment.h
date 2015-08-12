#ifndef RL_EXPERIMENT_H
#define RL_EXPERIMENT_H

#include <string>

#include "Scene.h"


#include "RLAgent.h"
#include "RLEnvironment.h"

#include "LifeSim.pb.h"


using namespace std;

using namespace LifeSim;

class RLExperiment {

  RLAgent       *agent;
  RLEnvironment *env;

  RLStateDesc   lastState;
  RLActionDesc  lastAction;
  float         reward;

  RLExperimentDesc desc;

  // envStep increments every environment step, and wraps to 0 after numEnvStepsPerRLStep
  int   envStep;
  float remainingTime;

  bool isEpisodeStart;

public:

  RLExperiment();
  ~RLExperiment();

	void load(const string &filename);

  void start();
  void step(float dt);
  void stepRL(void);

  Scene* getScene() {
    return env->getScene();
  }

};


#endif // RL_EXPERIMENT_H
