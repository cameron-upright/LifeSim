
#include <glog/logging.h>

#include <stdio.h>
//#include <unistd.h>     // needed to sleep
#include <math.h>
#include <iostream>

#include <thread>

#include "timer.h"

#include "Scene.h"

#include "CreatureEnv.h"

#include "RLGlue/RLGlue++.h"
#include "RLGlue/EnvServer.h"


using namespace std;

Timer timer;

CreatureEnv env;

std::mutex quitMutex;
bool quit = false;

// Total time simulated
double simulatedTime = 0.0;

// Total time running
double runTime = 0.0;

void stepSim() {

	// Find the elapsed time since the last step
  timer.update();
  float elapsed = timer.get_dif_f();

	// Update the total time
	runTime += elapsed;


  timer.reset();

	// Step the sim
	float simTime = env.stepSim(elapsed * 5.0);

	simulatedTime += simTime;

	/*
	if (runTime > 0.0 &&
			(ceil(simulatedTime - simTime) != ceil(simulatedTime)))
		LOG(INFO) << "Speed " << (simulatedTime / runTime);
	*/

}

bool init(int argc, char **argv) {

	// Check the args
	if (argc != 2) {
		LOG(ERROR) << "Usage :" << endl << " ./gui [env.prototxt]";
		return false;
	}

  // default random seeds
  srand48(0);
  srand(0);

	// Load the environment
	env.load(argv[1]);

	// Discount factor which decays 0.5 in simSpeedDiscountHalfLife seconds
	//	simSpeedDiscountFactor = pow(0.5, 1.0 / ceil(simSpeedDiscountHalfLife / env.getEnvStepSize()));

	return true;

}


void mainLoop() {

	timer.reset();

	while (true) {

		// Quit maybe
		{
			std::lock_guard<std::mutex> guard(quitMutex);
			if (quit)
				break;
		}

		stepSim();

	}

}





void rlLoop() {

  try {
		boost::asio::io_service io_service;
		RLGlue::EnvServer server(io_service, env);
		io_service.run();
	}

  catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	{
		std::lock_guard<std::mutex> guard(quitMutex);
		quit = true;
	}

}

int main(int argc, char **argv) {

  FLAGS_alsologtostderr = 1;

	google::InitGoogleLogging(argv[0]);


	if (!init(argc, argv))
		return 1;


	std::thread rlThread(rlLoop);

	mainLoop();

	rlThread.join();

	return 0;

}



/*

std::thread mainThread;


//Observation observation;
//RewardObservationTerminal rewardObservationTerminal;


observation_t this_observation;

std::string taskSpec;

bool firstInit = true;

int step = 0;


const char* env_init() {

	cerr << "ENV env_init start" << endl;

//	const char* task_spec="VERSION RL-Glue-3.0 PROBLEMTYPE episodic DISCOUNTFACTOR 1.0 OBSERVATIONS INTS (0 20) ACTIONS INTS (0 1)  REWARDS (-1.0 1.0)  EXTRA skeleton_environment(C/C++) by Brian Tanner.";

	if (firstInit) {
		// Magic
		std::thread(init).join();

		// Start up the main loop
		mainThread = std::thread(glutMainLoop);

		firstInit = false;
	}

	// Allocate the observation variable 
	allocateRLStruct(&this_observation, 0, env.getCreature()->getNumDOF(), 0);

	// Setup the reward_observation variable 
 	rewardObservationTerminal = RewardObservationTerminal(0.0, &this_observation, 0);

	

//	observation = Observation(1,0,0);
//	rewardObservationTerminal = RewardObservationTerminal(0.0, observation.getObservationPtr(), 0);






	taskSpec  = "VERSION RL-Glue-3.0";
	taskSpec += " PROBLEMTYPE episodic";
	taskSpec += " DISCOUNTFACTOR 1.0";
	taskSpec += " OBSERVATIONS";
	taskSpec += " DOUBLES (" + to_string(env.getCreature()->getNumDOF()) + " 0 1)";
	taskSpec += " ACTIONS";
	taskSpec += " DOUBLES (" + to_string(env.getCreature()->getNumDOF()) + " 0 1)";
	taskSpec += " REWARDS (UNSPEC UNSPEC)";
	taskSpec += " EXTRA skeleton_environment(C/C++) by Brian Tanner.";


	cerr << "ENV env_init end" << endl;

	return taskSpec.c_str();

}


const observation_t *env_start() { 

	cerr << "ENV env_start start" << endl;

	int observationIndex = 0;
	for (unsigned i=0; i<env.getCreature()->hingeConstraints.size(); i++)
		this_observation.doubleArray[observationIndex++] = env.getCreature()->hingeConstraints[i]->getAngle();
	for (unsigned i=0; i<env.getCreature()->universalConstraints.size(); i++) {
		this_observation.doubleArray[observationIndex++] = env.getCreature()->universalConstraints[i]->getAngle(0);
		this_observation.doubleArray[observationIndex++] = env.getCreature()->universalConstraints[i]->getAngle(1);
	}

	cerr << "ENV env_start end" << endl;

	step = 0;

	//	return observation.getObservationPtr();
	return &this_observation;

}


const reward_observation_terminal_t *env_step(const action_t *this_action) {

	//	cerr << "ENV env_step start" << endl;

	float the_reward=0;
	int episode_over=0;


	// Prepare the step, creating an action to resist movement
	LifeSim::StateDesc state;
	LifeSim::RLActionDesc action;

	const float constraintMultiplier = 1.0f;

	int actionInd = 0;
	for (unsigned i=0; i<env.getCreature()->hingeConstraints.size(); i++)
		action.add_action(-constraintMultiplier * this_action->doubleArray[actionInd++]);
	for (unsigned i=0; i<env.getCreature()->universalConstraints.size(); i++) {
		action.add_action(-constraintMultiplier * this_action->doubleArray[actionInd++]);
		action.add_action(-constraintMultiplier * this_action->doubleArray[actionInd++]);
	}

	// Step the environment
	env.stepRL(state, action, the_reward);



	int observationIndex = 0;
	for (unsigned i=0; i<env.getCreature()->hingeConstraints.size(); i++)
		this_observation.doubleArray[observationIndex++] = env.getCreature()->hingeConstraints[i]->getAngle();
	for (unsigned i=0; i<env.getCreature()->universalConstraints.size(); i++) {
		this_observation.doubleArray[observationIndex++] = env.getCreature()->universalConstraints[i]->getAngle(0);
		this_observation.doubleArray[observationIndex++] = env.getCreature()->universalConstraints[i]->getAngle(1);
	}

	// Set episode_over
	step++;
	episode_over = step == 50;
	if (episode_over)
		step = 0;

	rewardObservationTerminal.setReward(the_reward);
	rewardObservationTerminal.setTerminal(episode_over);


	//	cerr << "ENV env_step end" << endl;
	return rewardObservationTerminal;

}


void env_cleanup() {

	cerr << "ENV env_cleanup start" << endl;

	clearRLStruct(&this_observation);

	cerr << "ENV env_cleanup end" << endl;
}


const char* env_message(const char* inMessage) {

	if(strcmp(inMessage,"what is your name?")==0)
		return "my name is skeleton_environment!";

	return "I don't know how to respond to your message";

}


*/
