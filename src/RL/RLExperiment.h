#ifndef RL_EXPERIMENT_H
#define RL_EXPERIMENT_H

#include <string>

#include "Scene.h"


#include "RLAgent.h"
#include "RLEnvironment.h"

#include "RLState.h"
#include "RLAction.h"


using namespace std;



class RLExperimentDescription {

public:

  // The number of times the environment is stepped, for every RL step
  int numEnvStepsPerRLStep;

};


class RLExperiment {

  RLAgent       *agent;
  RLEnvironment *env;

  RLState       lastState;
  RLAction      lastAction;
  float         reward;

  RLExperimentDescription desc;

  // envStep increments every environment step, and wraps to 0 after numEnvStepsPerRLStep
  int   envStep;
  float remainingTime;

  bool isEpisodeStart;

public:

  RLExperiment(RLEnvironment *env, RLAgent *agent, const RLExperimentDescription &desc);
  ~RLExperiment();

  void start();
  void step(float dt);
  void stepRL(void);

};


#endif // RL_EXPERIMENT_H
