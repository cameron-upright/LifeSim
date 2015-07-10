#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "Scene.h"

#include "RLState.h"
#include "RLAction.h"

class RLAgent {

  Creature *creature;

public:

  RLAgent() {}
  ~RLAgent() {}

  void step(const RLState &state, RLAction &action);

  void load(Scene *scene, const string &filename);


};

#endif // RL_AGENT
