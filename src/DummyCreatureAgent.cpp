
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
#include "Util/RLGlue/AgentServer.h"

#include "CreatureAgent.h"


using namespace std;
using boost::asio::ip::tcp;


int main(int argc, char **argv) {

  FLAGS_alsologtostderr = 1;

  google::InitGoogleLogging(argv[0]);

	if (argc != 2) {
		LOG(ERROR) << "Usage :" << endl << " ./DummyCreatureAgent [creature.prototxt]" << endl;
		return 1;
	}


	CreatureAgent agent(argv[1]);

  try {
		boost::asio::io_service io_service;
		RLGlue::AgentServer server(io_service, agent);
		io_service.run();
	}

  catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

  return 0;

}





/*
action_t this_action;
action_t last_action;

observation_t *last_observation = NULL;


enum TaskSpecParseStage {
	NIL,
	VERSION,
	PROBLEMTYPE,
	DISCOUNTFACTOR,
	OBSERVATIONS,
	ACTIONS,
	REWARDS,
	EXTRA
};



int randInRange(int max) {
	double r, x;
	r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
   	x = (r * (max+1));
	return (int)x;
}

void agent_init(const char* task_spec) {
	// Seed the random number generator

	cerr << "AGENT agent_init start" << endl;

	srand(time(0));
	// Here is where you might allocate storage for parameters (value function or policy, last action, last observation, etc)
	
	// Here you would parse the task spec if you felt like it
	string taskSpec(task_spec);

	vector<string> taskSpecParts = string_split(taskSpec, " ");



	TaskSpecParseStage parseStage = TaskSpecParseStage::NIL;
	string typeParseStage;

	int numDoubleObservations = 0;
	int numDoubleActions = 0;
	vector<double> doubleVals;

	// TODO : gross, no regex?
	for (int i=0; i<taskSpecParts.size(); i++) {

		const string &token = taskSpecParts[i];

		//		cout << "AGENT TOKEN " << token << endl;

		if (token == "VERSION") {
			parseStage = TaskSpecParseStage::VERSION;
		}
		else if (token == "PROBLEMTYPE") {
			parseStage = TaskSpecParseStage::PROBLEMTYPE;
		}
		else if (token == "DISCOUNTFACTOR") {
			parseStage = TaskSpecParseStage::DISCOUNTFACTOR;
		}
		else if (token == "OBSERVATIONS") {
			parseStage = TaskSpecParseStage::OBSERVATIONS;
		}
		else if (token == "ACTIONS") {
			parseStage = TaskSpecParseStage::ACTIONS;
		}
		else if (token == "REWARDS") {
			parseStage = TaskSpecParseStage::REWARDS;
		}
		else if (token == "EXTRA") {
			parseStage = TaskSpecParseStage::EXTRA;

		} else {

			if (token == "DOUBLES") {
				typeParseStage = "DOUBLES";
				doubleVals.clear();
			}

			else if (typeParseStage == "DOUBLES") {

				string val;

				// TODO : gross
				if (token[0] == '(') {
					val = token.substr(1, token.length());
				} else if (token[token.length()-1] == ')') {
					val = token.substr(0, token.length()-1);
					typeParseStage = "";
				} else
					val = token;

				doubleVals.push_back(stod(val));

				if (token[token.length()-1] == ')') {

					if (parseStage == TaskSpecParseStage::OBSERVATIONS)
						numDoubleObservations = (int)(doubleVals.size() == 3 ? doubleVals[0] : 1);
					else
						numDoubleActions = (int)(doubleVals.size() == 3 ? doubleVals[0] : 1);

				}

			}

		}
	}



	cerr << "AGENT task_spec " << task_spec << endl;

	// Allocate memory for a one-dimensional integer action using utility functions from RLStruct_util
	allocateRLStruct(&this_action,0,numDoubleObservations,0);
	last_observation=allocateRLStructPointer(0,0,0);

	cerr << "AGENT agent_init done" << endl;
	
	// That is equivalent to:
	//			 this_action.numInts     =  1;
	//			 this_action.intArray    = (int*)calloc(1,sizeof(int));
	//			 this_action.numDoubles  = 0;
	//			 this_action.doubleArray = 0;
	//			 this_action.numChars    = 0;
	//			 this_action.charArray   = 0;

}

const action_t *agent_start(const observation_t *this_observation) {

	cerr << "AGENT agent_start start" << endl;

	//  This agent always returns zero double actions 
	for (int i=0; i<this_action.numDoubles; i++)
		this_action.doubleArray[i] = drand48()-0.5;

	//  In a real action you might want to store the last observation and last action
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);

	cerr << "AGENT agent_start done" << endl;
	
	return &this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {


	cerr << "AGENT agent_step start" << endl;

	//  This agent always returns zero double actions 
	for (int i=0; i<this_action.numDoubles; i++)
		this_action.doubleArray[i] = drand48()-0.5;

	//  In a real action you might want to store the last observation and last action
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);

	//	cerr << "AGENT agent_step done" << endl;
	
	return &this_action;
}

void agent_end(double reward) {

	cerr << "AGENT agent_end start" << endl;

	clearRLStruct(&last_action);
	clearRLStruct(last_observation);

	cerr << "AGENT agent_end done" << endl;

}

void agent_cleanup() {
	cerr << "AGENT agent_cleanup start" << endl;

	clearRLStruct(&this_action);
	clearRLStruct(&last_action);
	freeRLStructPointer(last_observation);

	cerr << "AGENT agent_cleanup done" << endl;

}

const char* agent_message(const char* inMessage) {
	if(strcmp(inMessage,"what is your name?")==0)
		return "my name is skeleton_agent!";

	return "I don't know how to respond to your message";
}
*/
