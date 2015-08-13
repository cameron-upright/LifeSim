#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "Scene.h"

#include "LifeSim.pb.h"



class RLAgent {

protected:

  Creature *creature;

public:

  RLAgent() : creature(NULL) {}
  virtual ~RLAgent() {}

  void load(Scene *scene, const string &filename);

  virtual void step(const LifeSim::RLStateDesc &state, LifeSim::RLActionDesc &action) = 0;

};

#endif // RL_AGENT
