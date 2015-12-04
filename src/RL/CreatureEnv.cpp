#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <glog/logging.h>

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

  int fd = open(filename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc_);

	close(fd);

	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

	scenePath_ = dir + "/" + desc_.scene();

	scene->load(scenePath_.c_str());
	creature = scene->getCreature(desc_.creature());

	envStepSize = desc_.env_step_size();
	envStepPerRLStep = desc_.env_step_per_rl_step();

}



RLGlue::StateDesc CreatureEnv::start() {

	LOG(INFO) << "ENV start start";

	std::unique_lock<std::mutex> simLock(simMutex);
	std::unique_lock<std::mutex> rlLock(rlMutex);

	remainingTime = 0.0f;
	envStep = 0;



	// Restart the scene
	scene->load(scenePath_.c_str());
	creature = scene->getCreature(desc_.creature());

	// Update the currentState
	updateCurrentState();

	//	LOG(INFO) << "ENV start done";

	return *currentState;

}



RLGlue::RewardStateTerminal CreatureEnv::step(const RLGlue::ActionDesc &action) {

	RLGlue::ActionDesc actionCopy = action;

	// Prepare the step, creating an action to resist movement
	RLGlue::StateDesc state;
	float theReward=0;

	// Step the environment
	stepRL(state, actionCopy, theReward);

	Vector3f upDirection = creature->getUpDirection();
	bool isTerminal = upDirection.dot(Vector3f(0,1,0)) < cos(3.0f / 8.0f * M_PI);

	RLGlue::RewardStateTerminal rst;

	rst.set_reward(theReward);
	*(rst.mutable_state()) = state;
	rst.set_terminal(isTerminal);

	return rst;

}


float CreatureEnv::stepSim(float dt) {

	std::unique_lock<std::mutex> simLock(simMutex);
	std::unique_lock<std::mutex> rlLock(rlMutex);

	// If there's no action yet, return
	if (currentAction.get() == nullptr)
		return 0.0f;

	float simulated = 0.0f;

	// Update the remaining time to be simulated
	remainingTime += dt;

	//	LOG(INFO) << "ENV stepSim stepSim " << remainingTime << " " << envStep;

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

		simulated += envStepSize;

	}

	// Done stepping the simulation for this action
	if (envStep == envStepPerRLStep) {

		currentAction.release();
		updateCurrentState();

		Vector3f velocity = creature->getVelocity();
		Vector3f forward(0.0f, 0.0f, 1.0f);

		currentReward = velocity.dot(forward);

		// Notify the RL that the step is done
		stepCond.notify_one();

	}

	//	LOG(INFO) << "ENV stepSim end";

	return simulated;

}

// void stepRL(RLGlue::StateDesc &state, const RLGlue::ActionDesc &action, float &reward);
void CreatureEnv::stepRL(StateDesc &state, const ActionDesc &action, float &reward) {

	//	LOG(INFO) << "ENV stepRL start";

	// Set the new action
	// This assumes that the simulation is waiting for the action
	{
		// The lock
		std::lock_guard<std::mutex> lock(rlMutex);

		// The simulation should be waiting for an action at this point
		CHECK(currentAction.get() == nullptr);

		// Set the action
		currentAction.reset(new ActionDesc(action));
	}

	//	LOG(INFO) << "ENV stepRL wait start";


	// Wait until the simulation is done, and we have a new state and reward
	{
		std::unique_lock<std::mutex> stepLock(stepMutex);
		stepCond.wait(stepLock);
	}

	//	LOG(INFO) << "ENV stepRL wait done";

	// Wait for access to the member variables, then copy the state / reward, and reset the envStep
	{
		std::lock_guard<std::mutex> lock(rlMutex);
		state = *currentState;
		reward = currentReward;

		envStep = 0;
	}

	//	LOG(INFO) << "ENV stepRL done";

}


void CreatureEnv::updateCurrentState() {

	currentState.reset(new StateDesc());

	for (auto hingeConstraint : creature->hingeConstraints)
		currentState->add_float_state(hingeConstraint->getAngle());

	for (auto universalConstraint : creature->universalConstraints) {
		currentState->add_float_state(universalConstraint->getAngle(0));
		currentState->add_float_state(universalConstraint->getAngle(1));
	}


}

void CreatureEnv::applyControl(const ActionDesc &action) {

	//	const float constraintMultiplier = 0.0f;
	const float constraintStrength = 0.0001;

	int numAction = (int)(creature->hingeConstraints.size() + creature->universalConstraints.size() * 2);

	CHECK_EQ(action.float_action_size(), numAction);


	int actionIndex = 0;

	for (auto hingeConstraint : creature->hingeConstraints)
		hingeConstraint->enableAngularMotor(true, action.float_action(actionIndex++), constraintStrength);

	for (auto universalConstraint : creature->universalConstraints) {
		universalConstraint->enableAngularMotor(0, true, action.float_action(actionIndex++), constraintStrength);
		universalConstraint->enableAngularMotor(1, true, action.float_action(actionIndex++), constraintStrength);
	}


}

