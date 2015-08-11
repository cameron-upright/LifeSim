#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "Scene.h"

#include "RLAction.h"

class RLEnvironment {

  Scene *scene;
  Creature *creature;

	float envStepSize;

public:

  RLEnvironment();
  ~RLEnvironment();

  void load(const string &filename);

  void start(LifeSim::RLStateDesc &state);
  void step(LifeSim::RLStateDesc &state, const RLAction &action, float &reward, float dt);

  Scene* getScene() {
    return scene;
  }

	float getEnvStepSize() const {
		return envStepSize;
	}

private:

  void applyControl(const RLAction &action);

};




#endif // RL_ENVIRONMENT_H
