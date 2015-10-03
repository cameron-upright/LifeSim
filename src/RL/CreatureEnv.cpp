#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "CreatureEnv.h"

#include "RLGlue.pb.h"
#include "LifeSim.pb.h"


using namespace RLGlue;


CreatureEnv::CreatureEnv() {

  scene = new Scene();

	remainingTime = 0.0f;
	envStep = 0;

}

CreatureEnv::~CreatureEnv() {
  delete scene;
}


void CreatureEnv::load(const string &filename) {

	LifeSim::CreatureEnvDesc desc;

  int fd = open(filename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);
	
	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

	scene->load((dir + "/" + desc.scene()).c_str());
	creature = scene->getCreature(desc.creature());

	envStepSize = desc.env_step_size();
	envStepPerRLStep = desc.env_step_per_rl_step();

}



void CreatureEnv::start(RLStateDesc &state) {

	//	cerr << "ENV start start" << endl;

	std::unique_lock<std::mutex> simLock(simMutex);
	std::unique_lock<std::mutex> rlLock(rlMutex);

	remainingTime = 0.0f;
	envStep = 0;

	//	cerr << "ENV start done" << endl;

}



void CreatureEnv::stepSim(float dt) {

	//	cerr << "ENV stepSim start" << endl;

	std::unique_lock<std::mutex> simLock(simMutex);
	std::unique_lock<std::mutex> rlLock(rlMutex);

	// If there's no action yet, return
	if (currentAction.get() == nullptr)
		return;

	// Update the remaining time to be simulated
	remainingTime += dt;

	//	cerr << "ENV stepSim sim " << remainingTime << " " << envStep << endl;

	// false false, return, no time to simulate, and no action
	// false true,  return, action ready but no time to simulate
	// true  false, return, time to simulate but nothing to do
	// true  true,  simulate the time, if we finish the current action, 
	//              update our state and notify the stepRL

	// Simulate as long as there's remaining simulation time,
  // we're not done our env steps per RL step,
	// and we have an action to do
	while ((remainingTime > envStepSize) && envStep < envStepPerRLStep) {

		// Make sure the controller is up to date
		applyControl(*currentAction);

		// Step
		scene->step(envStepSize);
		remainingTime -= envStepSize;
		envStep++;

	}

	// Done stepping the simulation for this action
	if (envStep == envStepPerRLStep) {

		currentAction.release();
		currentState.reset(new RLStateDesc());
		currentReward = 0.0f;

		// Notify the RL that the step is done
		stepCond.notify_one();

	}

	//	cerr << "ENV stepSim end" << endl;

}

// void stepRL(RLGlue::RLStateDesc &state, const RLGlue::RLActionDesc &action, float &reward);
void CreatureEnv::stepRL(RLStateDesc &state, const RLActionDesc &action, float &reward) {

	/*
  Scene *scene;
  Creature *creature;
	float remainingTime;
  int envStep;

  unique_ptr<RLGlue::RLStateDesc>  currentState;
  unique_ptr<RLGlue::RLActionDesc> currentAction;
	float currentReward;
	*/

	//	cerr << "ENV stepRL start" << endl;

	// Set the new action
	// This assumes that the simulation is waiting for the action
	{
		// The lock
		std::lock_guard<std::mutex> lock(rlMutex);

		// The simulation should be waiting for an action at this point
		assert(currentAction.get() == nullptr);

		// Set the action
		currentAction.reset(new RLActionDesc(action));
	}

	//	cerr << "ENV stepRL wait start" << endl;


	// Wait until the simulation is done, and we have a new state and reward
	{
		std::unique_lock<std::mutex> stepLock(stepMutex);
		stepCond.wait(stepLock);
	}

	//	cerr << "ENV stepRL wait done" << endl;

	// Wait for access to the member variables, then copy the state / reward, and reset the envStep
	{
		std::lock_guard<std::mutex> lock(rlMutex);
		state = *currentState;
		reward = currentReward;

		envStep = 0;
	}

	//	cerr << "ENV stepRL done" << endl;

}




void CreatureEnv::applyControl(const RLActionDesc &action) {

	const float constraintMultiplier = 0.0f;
	const float constraintStrength = 0.0001;

	assert(action.action_size() == creature->hingeConstraints.size() + creature->universalConstraints.size() * 2);


	int actionIndex = 0;

	for (auto hingeConstraint : creature->hingeConstraints)
		hingeConstraint->enableAngularMotor(true, action.action(actionIndex++), constraintStrength);

	for (auto universalConstraint : creature->universalConstraints) {
		universalConstraint->enableAngularMotor(0, true, action.action(actionIndex++), constraintStrength);
		universalConstraint->enableAngularMotor(1, true, action.action(actionIndex++), constraintStrength);
	}


}

