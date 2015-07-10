#include <fstream>
#include <istream>
#include <sstream>

#include "Experiment.h"

#include "RLAgent.h"



Experiment::Experiment() {

	// Need to create this before we load the config file,
	// since we need to setup the scene observer before we load the scene
	env = new RLEnvironment();

	agent = NULL;

	rlExperiment = NULL;

}


Experiment::~Experiment() {

	if (agent)
		delete agent;

	if (env)
		delete env;

	if (rlExperiment)
		delete rlExperiment;

}


void Experiment::load(const string &filename) {

	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

  ifstream fin(filename.c_str());

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);

	RLExperimentDescription desc;

	string type;
	string envConfig;
	string agentConfig;
	string agentCreature;


	// Only RLExperiment is supported for now
	doc["type"] >> type;
	assert(type == "rl_experiment");

	// Grab the config info we need to load the environment and agent
	doc["env"] >> envConfig;
	doc["agent"] >> agentConfig;

	doc["numSecondsPerEnvStep"] >> desc.numSecondsPerEnvStep;
	doc["numEnvStepsPerRLStep"] >> desc.numEnvStepsPerRLStep;

	// Load the environment
	env->load(dir + "/" + envConfig);

	// Load the agent
	agent = new RLAgent();

	agent->load(env->getScene(), dir + "/" + agentConfig);


	fin.close();


	rlExperiment = new RLExperiment(env, agent, desc);

	rlExperiment->start();

  //  rlExperiment->load(filename);

}


void Experiment::step(float dt) {

	//	env->step(dt);

	rlExperiment->step(dt);

}



