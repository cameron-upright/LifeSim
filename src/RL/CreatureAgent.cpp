#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "CreatureAgent.h"

#include "RLGlue.pb.h"
#include "LifeSim.pb.h"


using namespace RLGlue;


CreatureAgent::CreatureAgent() {}
CreatureAgent::~CreatureAgent() {}


RLGlue::ActionDesc CreatureAgent::start(const RLGlue::StateDesc &state) {
	return ActionDesc();
}
RLGlue::ActionDesc CreatureAgent::step(const RLGlue::RewardState &rewardState) {
	return ActionDesc();
}

void CreatureAgent::end() {}
