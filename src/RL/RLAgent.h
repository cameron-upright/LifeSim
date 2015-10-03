#ifndef RL_AGENT_H
#define RL_AGENT_H

#include <memory>

#include "Scene.h"

#include "RLGlue.pb.h"



class RLAgent {

public:

  Creature *creature;

	RLAgent() = delete;
	RLAgent(Scene *scene);

  virtual ~RLAgent() {}

  static std::unique_ptr<RLAgent> load(Scene *scene, const string &filename);

  virtual void step(const RLGlue::RLStateDesc &state, RLGlue::RLActionDesc &action) = 0;

};

#endif // RL_AGENT
