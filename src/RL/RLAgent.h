#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "Scene.h"

#include "RLAction.h"

#include "LifeSim.pb.h"



class RLAgent {

  Creature *creature;

public:

  RLAgent() {}
  ~RLAgent() {}

  void step(const LifeSim::RLStateDesc &state, LifeSim::RLActionDesc &action);

  void load(Scene *scene, const string &filename);


};

#endif // RL_AGENT
