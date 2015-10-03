#ifndef RL_GLUE_XX_H
#define RL_GLUE_XX_H

#include <google/protobuf/message.h>
#include <boost/asio.hpp>


namespace RLGlue {

	void writeMessage(boost::asio::ip::tcp::socket &socket, const ::google::protobuf::Message &msg);

};


#endif // RL_GLUE_CXX_H

