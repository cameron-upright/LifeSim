#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "Scene.h"

#include "LifeSim.pb.h"


class RLEnvironment {

  Scene *scene;
  Creature *creature;

	float envStepSize;

public:

  RLEnvironment();
  ~RLEnvironment();

  void load(const string &filename);

  void start(LifeSim::RLStateDesc &state);
  void step(LifeSim::RLStateDesc &state, const LifeSim::RLActionDesc &action, float &reward, float dt);

  Scene* getScene() {
    return scene;
  }

	float getEnvStepSize() const {
		return envStepSize;
	}

private:

  void applyControl(const LifeSim::RLActionDesc &action);

};




#endif // RL_ENVIRONMENT_H
