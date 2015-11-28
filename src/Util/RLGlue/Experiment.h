#ifndef RL_GLUE_EXPERIMENT_H
#define RL_GLUE_EXPERIMENT_H

#include "RLGlue++.h"

#include "Util/RLGlue/EnvClient.h"
#include "Util/RLGlue/AgentClient.h"


namespace RLGlue {

	class Experiment {

		EnvClient   &envClient_;
		AgentClient &agentClient_;

	public:

		Experiment(EnvClient &envClient, AgentClient &agentClient) :
			envClient_(envClient), agentClient_(agentClient) {}

		bool runEpisode(const int stepLimit = std::numeric_limits<int>::max());

	};

}

#endif // RL_GLUE_EXPERIMENT_H
