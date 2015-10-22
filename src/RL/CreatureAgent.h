#ifndef CREATURE_AGENT_H
#define CREATURE_AGENT_H

#include <memory>
#include <mutex>
#include <condition_variable>

#include "Scene.h"

#include "RLGlue/RLGlue++.h"
#include "RLGlue.pb.h"


class CreatureAgent : public RLGlue::Agent {

public:

  CreatureAgent();
  ~CreatureAgent();

	RLGlue::ActionDesc start();
	RLGlue::ActionDesc step();
	void end();


private:

};




#endif // CREATURE_AGENT_H
