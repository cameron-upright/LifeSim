#ifndef CREATURE_AGENT_H
#define CREATURE_AGENT_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>

#include "Scene.h"

#include "RLGlue/RLGlue++.h"
#include "RLGlue.pb.h"

#include "LifeSim.pb.h"

class CreatureAgent : public RLGlue::Agent {

public:

  CreatureAgent(const string &creatureFile);
  ~CreatureAgent();

	RLGlue::ActionDesc start(const RLGlue::StateDesc &state);
	RLGlue::ActionDesc step(const RLGlue::RewardState &rewardState);
	void end(const float &reward);


private:

	LifeSim::SceneCreatureDesc creatureDesc;

	RLGlue::ActionDesc prevAction;

};




#endif // CREATURE_AGENT_H
