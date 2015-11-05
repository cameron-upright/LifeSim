#ifndef RL_GLUE_AGENT_CLIENT_H
#define RL_GLUE_AGENT_CLIENT_H

#include <iostream>
using namespace std;

#include "RLGlue++.h"

namespace RLGlue {

	class AgentClient {
	public:
		AgentClient(boost::asio::io_service& io_service,
								const std::string &host,
								const std::string &service) :
			socket_(io_service) {

			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(host, service);
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			boost::asio::connect(socket_, endpoint_iterator);

		}

		void init() {

			// Initialize the agent
			RLGlue::AgentCommand initCmd;
			initCmd.set_type(RLGlue::AgentCommand_Type_AGENT_INIT);
		
			RLGlue::writeMessage(socket_, initCmd);

		}

		ActionDesc start(const StateDesc &state) {

			// Start the agent
			AgentCommand cmd;
			cmd.set_type(AgentCommand_Type_AGENT_START);

			StateDesc stateCopy(state);

			AgentCommand_StartCommand *startCmd = cmd.mutable_startcommand();
			*(startCmd->mutable_state()) = stateCopy;

			writeMessage(socket_, cmd);

			return readMessage<ActionDesc>(socket_);

		}

		ActionDesc step() {
		
			// Write a step command
			AgentCommand stepCmd;
			stepCmd.set_type(RLGlue::AgentCommand_Type_AGENT_STEP);

			writeMessage(socket_, stepCmd);

			return readMessage<ActionDesc>(socket_);


		}


		void end(float reward) {}

		void cleanup() {

			// Cleanup the agent
			RLGlue::AgentCommand cleanupCmd;
			cleanupCmd.set_type(RLGlue::AgentCommand_Type_AGENT_CLEANUP);

			writeMessage(socket_, cleanupCmd);

		}


		boost::asio::ip::tcp::socket socket_;

	};

}

#endif // RL_GLUE_AGENT_CLIENT_H
