#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <string>

#include "RLExperiment.h"

using namespace std;


class Experiment {

public:

  RLExperiment *rlExperiment;

  RLAgent       *agent;
  RLEnvironment *env;


  Experiment();
  ~Experiment();

  void load(const string &filename);

  void step(float dt);

  Scene* getScene() {
    return env->getScene();
  }


};


#endif // EXPERIMENT_H
