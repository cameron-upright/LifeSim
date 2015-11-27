
#include <ctime>

#include <iostream>
#include <vector>

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <glog/logging.h>

#include "Proto/LifeSim.pb.h"
#include "Util/RLGlue/RLGlue++.h"
#include "Util/RLGlue/Experiment.h"

using namespace std;
using boost::asio::ip::tcp;





int main(int argc, char **argv) {

  FLAGS_alsologtostderr = 1;

  google::InitGoogleLogging(argv[0]);

  try {

    boost::asio::io_service io_service;

		// Create the env and agent clients
		RLGlue::EnvClient envClient(io_service, argv[1], "1337");
		RLGlue::AgentClient agentClient(io_service, argv[1], "1338");

		// Init
		envClient.init();
		agentClient.init();

		// Create our experiment
		RLGlue::Experiment experiment(envClient, agentClient);

		// Run a single 150 step episode, 100 times
		for (int i=0; i<100; i++)
			experiment.runEpisode(150);

		// Cleanup
		envClient.cleanup();
		agentClient.cleanup();

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

  return 0;

}





/*
#include <stdio.h>
#include <rlglue/RL_glue.h>
	
int whichEpisode=0;

void runEpisode(int stepLimit) {        
    int terminal=RL_episode(stepLimit);
	printf("Episode %d\t %d steps \t%f total reward\t %d natural end \n",whichEpisode,RL_num_steps(),RL_return(), terminal);
	whichEpisode++;
}

int main(int argc, char *argv[]) {
	const char* task_spec;
	const char* responseMessage;
	const reward_observation_action_terminal_t *stepResponse;
	const observation_action_t *startResponse;

	printf("\n\nExperiment starting up!\n");


	task_spec=RL_init();
	printf("RL_init called, the environment sent task spec: %s\n",task_spec);

	printf("\n\n----------Sending some sample messages----------\n");

	responseMessage=RL_agent_message("what is your name?");
	printf("Agent responded to \"what is your name?\" with: %s\n",responseMessage);
	responseMessage=RL_agent_message("If at first you don't succeed; call it version 1.0");
	printf("Agent responded to \"If at first you don't succeed; call it version 1.0\" with: %s\n\n",responseMessage);

	responseMessage=RL_env_message("what is your name?");
	printf("Environment responded to \"what is your name?\" with: %s\n",responseMessage);
	responseMessage=RL_env_message("If at first you don't succeed; call it version 1.0");
	printf("Environment responded to \"If at first you don't succeed; call it version 1.0\" with: %s\n",responseMessage);

	printf("\n\n----------Running a few episodes----------\n");
	while (true)
		runEpisode(100);

	// Remember that stepLimit of 0 means there is no limit at all!
	runEpisode(0);

	RL_cleanup();

	printf("\n\n----------Stepping through an episode----------\n");

	task_spec=RL_init();

	startResponse=RL_start();
	printf("First observation and action were: %d %d\n",startResponse->observation->intArray[0],startResponse->action->intArray[0]);

	stepResponse=RL_step();
	
	while(stepResponse->terminal!=1){
		stepResponse=RL_step();
		if(stepResponse->terminal!=1){
		}
	}
	
	printf("\n\n----------Summary----------\n");
	

	printf("It ran for %d steps, total reward was: %f\n",RL_num_steps(),RL_return());
	RL_cleanup();


	return 0;
}
*/
