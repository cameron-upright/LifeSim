#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "RLAgent.h"
#include "RLAgentDummy.h"


RLAgent::RLAgent(Scene *scene) {

	// TODO : hack
	creature = scene->getCreature("creature");

}


std::unique_ptr<RLAgent> RLAgent::load(Scene *scene, const string &filename) {

	//	return RLAgentDummy(scene, filename);

	// Get the directory of the file
	string filenameStr = string(filename);
	size_t found;
	found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);


	LifeSim::RLAgentDesc desc;

  int fd = open(filename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);


	if (desc.type() == LifeSim::RLAgentDesc_Type_DUMMY)
		return std::unique_ptr<RLAgent>(new RLAgentDummy(scene));

	abort();

}


