#ifndef RL_GLUE_ENV_SERVER_H
#define RL_GLUE_ENV_SERVER_H

#include "RLGlue++.h"

namespace RLGlue {

	class EnvServerConnection : public boost::enable_shared_from_this<EnvServerConnection> {

	public:
		typedef boost::shared_ptr<EnvServerConnection> pointer;

		static pointer create(boost::asio::io_service& io_service, Env &env) {
			return pointer(new EnvServerConnection(io_service, env));
		}

		boost::asio::ip::tcp::socket& socket() {
			return socket_;
		}

		void start() {
			readCommand();
		}

	private:
		EnvServerConnection(boost::asio::io_service& io_service, Env &env)
			: socket_(io_service), env_(env) {}

		void readCommand() {

			headerReadBuffer_.resize(1);

			boost::asio::async_read(socket_, boost::asio::buffer(headerReadBuffer_),
															boost::bind(&EnvServerConnection::handleReadCommand, shared_from_this(),
																					boost::asio::placeholders::error,
																					boost::asio::placeholders::bytes_transferred));

		}


		// TODO : Refactor to an asyncReadCommand function
		void handleReadCommand(const boost::system::error_code& error, size_t num_bytes) {

			if (error)
				return;

			bodyReadBuffer_.resize(headerReadBuffer_[0]);

			boost::asio::async_read(socket_, boost::asio::buffer(bodyReadBuffer_),
															boost::bind(&EnvServerConnection::handleReadCommandBody, shared_from_this(),
																					boost::asio::placeholders::error,
																					boost::asio::placeholders::bytes_transferred));

		}



		void handleReadCommandBody(const boost::system::error_code& error, size_t num_bytes) {

			if (error)
				return;

			std::string cmdBodyStr(bodyReadBuffer_.begin(), bodyReadBuffer_.end());

			RLGlue::EnvironmentCommand cmd;


			cmd.ParseFromString(cmdBodyStr);


			switch (cmd.type()) {


			case RLGlue::EnvironmentCommand_Type_ENV_INIT:

				// TODO : Initialize the environment

				// Wait for the next command
				readCommand();

				break;


			case RLGlue::EnvironmentCommand_Type_ENV_START:

				// Start a new episode, and send the start state back
				asyncWriteMessage(socket_, env_.start(), 
													boost::bind(&EnvServerConnection::handleWriteResponse, shared_from_this(),
																			boost::asio::placeholders::error,
																			boost::asio::placeholders::bytes_transferred));

				break;


			case RLGlue::EnvironmentCommand_Type_ENV_STEP:

				// Step the environment, and send the resulting RewardStateTerminal message
				asyncWriteMessage(socket_, env_.step(), 
													boost::bind(&EnvServerConnection::handleWriteResponse, shared_from_this(),
																			boost::asio::placeholders::error,
																			boost::asio::placeholders::bytes_transferred));

				break;


			case RLGlue::EnvironmentCommand_Type_ENV_CLEANUP:

				// TODO : Cleanup the environment

				// Wait for the next command
				readCommand();

				break;


			default:
				break;

			}

		}



		void handleWriteResponse(const boost::system::error_code& error, size_t num_bytes) {

			if (error)
				return;

			readCommand();

		}







		Env &env_;

		boost::asio::ip::tcp::socket socket_;

		std::vector<size_t> headerReadBuffer_;
		std::vector<char> bodyReadBuffer_;

	};






	class EnvServer {
	public:
		EnvServer(boost::asio::io_service& io_service, Env &env)
			: acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1337)),
				env_(env) {
			start_accept();
		}

	private:
		void start_accept() {
			RLGlue::EnvServerConnection::pointer new_connection =
				RLGlue::EnvServerConnection::create(acceptor_.get_io_service(), env_);

			acceptor_.async_accept(new_connection->socket(),
														 boost::bind(&EnvServer::handle_accept, this, new_connection,
																				 boost::asio::placeholders::error));
		}

		void handle_accept(RLGlue::EnvServerConnection::pointer new_connection,
											 const boost::system::error_code& error) {
			if (!error)
				{
					new_connection->start();
					start_accept();
				}
		}

		boost::asio::ip::tcp::acceptor acceptor_;

		Env &env_;

	};


}

#endif // RL_GLUE_ENV_SERVER_H
