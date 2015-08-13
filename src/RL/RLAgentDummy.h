#ifndef RL_AGENT_DUMMY_H
#define RL_AGENT_DUMMY_H

#include "LifeSim.pb.h"

#include "RLAgent.h"


class RLAgentDummy : public RLAgent {

public:

  RLAgentDummy(Scene *scene) : RLAgent(scene) {}

  ~RLAgentDummy() {}

  void step(const LifeSim::RLStateDesc &state, LifeSim::RLActionDesc &action);


};

#endif // RL_AGENT_DUMMY
