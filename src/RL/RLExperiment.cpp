#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <assert.h>

#include "RLExperiment.h"

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "LifeSim.pb.h"

using namespace LifeSim;


RLExperiment::RLExperiment() {

	// Need to create this before we load the config file,
	// since we need to setup the scene observer before we load the scene
	env = new RLEnvironment();
	agent = NULL;

	isEpisodeStart = true;

}


RLExperiment::~RLExperiment() {

	if (agent)
		delete agent;

	if (env)
		delete env;

}

void RLExperiment::load(const string &filename) {

	RLExperimentDesc desc2;

	// Load the protobuf
	int fd = open(filename.c_str(), O_RDONLY);
  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc2);
	close(fd);

	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

	const string &envConfig     = desc2.env_config();
	const string &agentConfig   = desc2.agent_config();

	this->desc.numEnvStepsPerRLStep = desc2.env_step_per_rl_step();

	// Load the environment
	env->load(dir + "/" + envConfig);

	// Load the agent
	agent = new RLAgent();

	agent->load(env->getScene(), dir + "/" + agentConfig);

	// TODO : Move out of here
	start();

}

void RLExperiment::start() {

	env->start(lastState);
	agent->step(lastState, lastAction);

	reward = 0.0f;

	envStep = 0;
	remainingTime = 0.0f;

}


void RLExperiment::step(float dt) {

	float stepsize;

	remainingTime += dt;

	// while there is more time to simulate
	while (remainingTime > env->getEnvStepSize()) {

		env->step(lastState, lastAction, reward, env->getEnvStepSize());
		++envStep %= desc.numEnvStepsPerRLStep;

		if (envStep == 0)
			stepRL();

		remainingTime -= env->getEnvStepSize();

	}

}


void RLExperiment::stepRL() {

	agent->step(lastState, lastAction);

}


