//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>

#include "src/Proto/LifeSim.pb.h"

#include "src/Util/RLGlue++.h"


using namespace std;

int main() {
  try
		{
			boost::asio::io_service io_service;
			RLGlue::EnvServer server(io_service);
			io_service.run();
			cout << "SERVER DONE RUN" << endl;
		}
  catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

  return 0;
}

