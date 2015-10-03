#include <string>
#include <vector>

#include <iostream>

#include "RL-Glue++.h"

using namespace std;


void RLGlue::writeMessage(boost::asio::ip::tcp::socket &socket, const ::google::protobuf::Message &msg) {

	std::string msgData;

	msg.SerializeToString(&msgData);

	cout << "CLIENT msgData " << (int)msgData[0] << " " << (int)msgData[1] << endl;

	cerr << "A" << endl;

	socket.write_some(boost::asio::buffer(std::vector<size_t>{msgData.size()}));
	cerr << "B" << endl;
	socket.write_some(boost::asio::buffer(msgData));
	cerr << "C" << endl;

}
/*
void RLGlue::readMessage(boost::asio::ip::tcp::socket &socket, const ::google::protobuf::Message &msg) {

	std::string msgData;

	msg.SerializeToString(&msgData);

	socket.write_some(boost::asio::buffer(std::vector<size_t>{msgData.size()}));
	socket.write_some(boost::asio::buffer(msgData));

}
*/
