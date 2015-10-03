//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <vector>

#include "src/Proto/LifeSim.pb.h"


using namespace std;

using boost::asio::ip::tcp;

std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}



class EnvConnection : public boost::enable_shared_from_this<EnvConnection> {

	//	public:

};


namespace RLGlue {

	class EnvConnection : public boost::enable_shared_from_this<EnvConnection> {

	public:
		typedef boost::shared_ptr<EnvConnection> pointer;

		static pointer create(boost::asio::io_service& io_service) {
			return pointer(new EnvConnection(io_service));
		}

		tcp::socket& socket() {
			return socket_;
		}

		void start() {

			readCommand();

		}

	private:
		EnvConnection(boost::asio::io_service& io_service)
			: socket_(io_service) {
		}

		void readCommand() {

			cmdReadBuffer_.resize(1);

			cout << "START READ COMMAND" << endl;	
			boost::asio::async_read(socket_, boost::asio::buffer(cmdReadBuffer_),
															boost::bind(&EnvConnection::handleReadCommand, shared_from_this(),
																					boost::asio::placeholders::error,
																					boost::asio::placeholders::bytes_transferred));
		}

		void handleReadCommand(const boost::system::error_code& error, size_t num_bytes) {

			cout << "SERVER HEADER " << (int)cmdReadBuffer_[0] << endl;

			cmdBodyReadBuffer_.resize(cmdReadBuffer_[0]);

			cout << "START READ COMMAND BODY" << endl;	
			boost::asio::async_read(socket_, boost::asio::buffer(cmdBodyReadBuffer_),
															boost::bind(&EnvConnection::handleReadCommandBody, shared_from_this(),
																					boost::asio::placeholders::error,
																					boost::asio::placeholders::bytes_transferred));
		}



		void handleReadCommandBody(const boost::system::error_code& error, size_t num_bytes) {

			std::string cmdBodyStr(cmdBodyReadBuffer_.begin(), cmdBodyReadBuffer_.end());

			LifeSim::RLEnvironmentCommand cmd;

			cmd.ParseFromString(cmdBodyStr);

			cout << "YAY " << (int)cmd.type() << endl;

			cout << "WRITE RESPONSE" << endl;

			/*
			cout << "WRITE" << endl;
			boost::asio::async_write(socket_, boost::asio::buffer(message_),
															 boost::bind(&EnvConnection::handle_write, shared_from_this(),
																					 boost::asio::placeholders::error,
																					 boost::asio::placeholders::bytes_transferred));
			*/

		}

		void handle_write(const boost::system::error_code& /*error*/,
											size_t /*bytes_transferred*/) {
		}

		tcp::socket socket_;
		std::string message_;
		std::vector<size_t> cmdReadBuffer_;
		std::vector<char> cmdBodyReadBuffer_;
	};


}


class EnvServer {
public:
  EnvServer(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1337)) {
    start_accept();
  }

private:
  void start_accept() {
    RLGlue::EnvConnection::pointer new_connection =
      RLGlue::EnvConnection::create(acceptor_.get_io_service());

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

  tcp::acceptor acceptor_;
};

int main() {
  try
		{
			boost::asio::io_service io_service;
			EnvServer server(io_service);
			io_service.run();
			cout << "SERVER DONE RUN" << endl;
		}
  catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

  return 0;
}

