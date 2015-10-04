//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include <ctime>

#include <iostream>
#include <vector>

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "src/Proto/LifeSim.pb.h"
#include "src/Util/RLGlue++.h"

using namespace std;
using boost::asio::ip::tcp;



using namespace std;

using boost::asio::ip::tcp;

class tcp_connection {

public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service, tcp::resolver::iterator &endpoint_iterator) {
    return pointer(new tcp_connection(io_service, endpoint_iterator));
  }

  tcp::socket& socket() {
    return socket_;
  }

private:


  tcp_connection(boost::asio::io_service& io_service, tcp::resolver::iterator &endpoint_iterator)
    : socket_(io_service) {

		cout << "CLIENT CONNECT START" << endl;
    boost::asio::connect(socket_, endpoint_iterator);
		cout << "CLIENT CONNECT DONE" << endl;

		start();

  }

  void start() {

		write_buffer_ = {42};

		cout << "CLIENT ASYNC WRITE START" << endl;

		//		auto sft = shared_from_this();
		//		cout << "WTF" << endl

    boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_),
														 boost::bind(&tcp_connection::handle_write, this,
																				 boost::asio::placeholders::error,
																				 boost::asio::placeholders::bytes_transferred));
		cout << "CLIENT ASYNC WRITE DONE" << endl;
  }

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {

		cout << "CLIENT READ" << endl;
    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket_.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }

	}

	void handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
		cout << "CLIENT HANDLE WRITE" << endl;

	}


  tcp::socket socket_;
  std::string message_;
	std::vector<int> write_buffer_;
};




int main(int argc, char **argv) {

  try {

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "1337");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		cout << "CLIENT CREATE START" << endl;

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);


		RLGlue::RLEnvironmentCommand cmd;
		RLGlue::RLEnvironmentCommand::InitCommand initCmd;

		cmd.set_type(RLGlue::RLEnvironmentCommand_Type_ENV_STEP);
		cmd.mutable_stepcommand();

		RLGlue::writeMessage(socket, cmd);

		/*

		vector<int> cmd = {42};
		socket.write_some(boost::asio::buffer(cmd));

		//		tcp_connection::pointer connection = tcp_connection::create(io_service, endpoint_iterator);

		cout << "CLIENT CREATE DONE" << endl;



    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
		*/
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	cout << "BLAH" << endl;

  return 0;
}

