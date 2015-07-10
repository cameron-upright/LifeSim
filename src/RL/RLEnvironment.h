#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "Scene.h"

#include "RLState.h"
#include "RLAction.h"

class RLEnvironment {

  Scene *scene;
  Creature *creature;

public:

  RLEnvironment();
  ~RLEnvironment();

  void load(const string &filename);

  void start(RLState &state);
  void step(RLState &state, const RLAction &action, float &reward, float dt);

  Scene* getScene() {
    return scene;
  }

private:

  void applyControl(const RLAction &action);

};




#endif // RL_ENVIRONMENT_H
