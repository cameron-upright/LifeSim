#ifndef RL_GLUE_XX_H
#define RL_GLUE_XX_H

#include <google/protobuf/message.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <iostream>


#include "../Proto/RLGlue.pb.h"

using namespace std;


namespace RLGlue {

	void writeMessage(boost::asio::ip::tcp::socket &socket, const ::google::protobuf::Message &msg);

	template <class MessageType>
	MessageType readMessage(boost::asio::ip::tcp::socket &socket) {

		// Read the header
		std::vector<size_t> headerReadBuffer(1);
		boost::asio::read(socket, boost::asio::buffer(headerReadBuffer));

		// Read the body
		std::vector<char> bodyReadBuffer(headerReadBuffer[0]);
		boost::asio::read(socket, boost::asio::buffer(bodyReadBuffer));

		// Convert to string, and then MessageType
		std::string msgData(bodyReadBuffer.begin(), bodyReadBuffer.end());

		MessageType msg;
		msg.ParseFromString(msgData);

		return msg;

	}


	class Env {
	public:

		virtual void init() {}

		virtual StateDesc start() = 0;

		virtual RewardStateTerminal step() = 0;

		virtual void cleanup() {}

	};


	class EnvConnection : public boost::enable_shared_from_this<EnvConnection> {

	public:
		typedef boost::shared_ptr<EnvConnection> pointer;

		static pointer create(boost::asio::io_service& io_service, Env &env) {
			return pointer(new EnvConnection(io_service, env));
		}

		boost::asio::ip::tcp::socket& socket() {
			return socket_;
		}

		void start() {
			readCommand();
		}

	private:
		EnvConnection(boost::asio::io_service& io_service, Env &env)
			: socket_(io_service), env_(env) {}

		void readCommand() {

			headerReadBuffer_.resize(1);

			boost::asio::async_read(socket_, boost::asio::buffer(headerReadBuffer_),
															boost::bind(&EnvConnection::handleReadCommand, shared_from_this(),
																					boost::asio::placeholders::error,
																					boost::asio::placeholders::bytes_transferred));
		}

		void handleReadCommand(const boost::system::error_code& error, size_t num_bytes) {

			if (error)
				return;

			bodyReadBuffer_.resize(headerReadBuffer_[0]);

			boost::asio::async_read(socket_, boost::asio::buffer(bodyReadBuffer_),
															boost::bind(&EnvConnection::handleReadCommandBody, shared_from_this(),
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

			case RLGlue::EnvironmentCommand_Type_ENV_STEP:

				{

					// Step the environment
					RewardStateTerminal rst = env_.step();

					// Serialize the response
					rst.SerializeToString(&messageWriteBuffer_);

					// Setup the buffers
					headerWriteBuffer_ = {messageWriteBuffer_.size()};
					bodyWriteBuffer_   = std::vector<char>(messageWriteBuffer_.begin(), messageWriteBuffer_.end());

					// Write the header size first
					boost::asio::async_write(socket_, boost::asio::buffer(headerWriteBuffer_),
																	 boost::bind(&EnvConnection::handleWriteResponseHeader, shared_from_this(),
																							 boost::asio::placeholders::error,
																							 boost::asio::placeholders::bytes_transferred));

					break;
				}


			default:
				break;

			}


		}

		void handleWriteResponseHeader(const boost::system::error_code& error, size_t num_bytes) {

			if (error)
				return;


			// Write the body, then when done read the next command
			boost::asio::async_write(socket_, boost::asio::buffer(bodyWriteBuffer_),
															 boost::bind(&EnvConnection::readCommand, shared_from_this()));

		}





		Env &env_;

		boost::asio::ip::tcp::socket socket_;

		std::vector<size_t> headerReadBuffer_;
		std::vector<char> bodyReadBuffer_;


		std::string messageWriteBuffer_;

		std::vector<size_t> headerWriteBuffer_;
		std::vector<char> bodyWriteBuffer_;

		

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
			RLGlue::EnvConnection::pointer new_connection =
				RLGlue::EnvConnection::create(acceptor_.get_io_service(), env_);

			acceptor_.async_accept(new_connection->socket(),
														 boost::bind(&EnvServer::handle_accept, this, new_connection,
																				 boost::asio::placeholders::error));
		}

		void handle_accept(RLGlue::EnvConnection::pointer new_connection,
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


#endif // RL_GLUE_CXX_H

