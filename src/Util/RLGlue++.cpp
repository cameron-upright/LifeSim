#include <string>
#include <vector>

#include <iostream>

#include "RLGlue++.h"

using namespace std;


void RLGlue::writeMessage(boost::asio::ip::tcp::socket &socket, const ::google::protobuf::Message &msg) {

	std::string msgData;

	msg.SerializeToString(&msgData);

	socket.write_some(boost::asio::buffer(std::vector<size_t>{msgData.size()}));
	socket.write_some(boost::asio::buffer(msgData));

}




