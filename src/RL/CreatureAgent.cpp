#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <glog/logging.h>

#include "CreatureAgent.h"


CreatureAgent::CreatureAgent(const string &creatureFilename) {
	
	LifeSim::SceneObjectDesc desc;

  int fd = open(creatureFilename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);

	CHECK_EQ(desc.type(), LifeSim::SceneObjectDesc_Type_CREATURE);

	creatureDesc = desc.GetExtension(LifeSim::SceneCreatureDesc::scene_object);

}


CreatureAgent::~CreatureAgent() {}


RLGlue::ActionDesc CreatureAgent::start(const RLGlue::StateDesc &state) {

	RLGlue::ActionDesc action;

	for (int i=0; i<42; i++)
		action.add_float_action(0.0);

	prevAction = action;

	return action;

}


RLGlue::ActionDesc CreatureAgent::step(const RLGlue::RewardState &rewardState) {

	//	LOG(INFO) << "step " << rewardState.reward();

	const float constraintMultiplier = 0.5f;

	RLGlue::ActionDesc action;

	for (auto a : prevAction.float_action()) {
		a *= 0.92;
		if (lrand48() % 10 == 0)
			a += 35.0*(drand48()-0.5);
		action.add_float_action(-constraintMultiplier * a);
	}

	prevAction = action;

	return action;

}

void CreatureAgent::end(const float &reward) {

	//	LOG(INFO) << "end " << reward;

}
