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
#include "src/Util/RLGlue/RLGlue++.h"

using namespace std;
using boost::asio::ip::tcp;



using namespace std;

using boost::asio::ip::tcp;


int main(int argc, char **argv) {

  try {

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "1337");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);



		// Initialize the environment
		RLGlue::EnvironmentCommand initCmd;

		//		initCmd.set_type(RLGlue::EnvironmentCommand_Type_ENV_INIT);

		//		RLGlue::writeMessage(socket, initCmd);


		// Step the environment
		while (true) {

			// Write a step command
			RLGlue::EnvironmentCommand stepCmd;

			stepCmd.set_type(RLGlue::EnvironmentCommand_Type_ENV_STEP);
			stepCmd.mutable_stepcommand()->mutable_action();

			RLGlue::writeMessage(socket, stepCmd);


			// Get the response
			RLGlue::RewardStateTerminal state = RLGlue::readMessage<RLGlue::RewardStateTerminal>(socket);

		}

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

  return 0;
}

