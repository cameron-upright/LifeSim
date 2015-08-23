/* 
* Copyright (C) 2008, Brian Tanner

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <rlglue/Agent_common.h>
#include <rlglue/utils/C/RLStruct_util.h>

#include <iostream>

using namespace std;

action_t this_action;
action_t last_action;

observation_t *last_observation = NULL;

int randInRange(int max) {
	double r, x;
	r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
   	x = (r * (max+1));
	return (int)x;
}

void agent_init(const char* task_spec) {
	/*Seed the random number generator*/

	cerr << "start agent_init" << endl;

	srand(time(0));
	/*Here is where you might allocate storage for parameters (value function or policy, last action, last observation, etc)*/
	
	/*Here you would parse the task spec if you felt like it*/
	
	/*Allocate memory for a one-dimensional integer action using utility functions from RLStruct_util*/
	allocateRLStruct(&this_action,1,0,0);
	last_observation=allocateRLStructPointer(0,0,0);

	cerr << "done agent_init" << endl;
	
	/* That is equivalent to:
			 this_action.numInts     =  1;
			 this_action.intArray    = (int*)calloc(1,sizeof(int));
			 this_action.numDoubles  = 0;
			 this_action.doubleArray = 0;
			 this_action.numChars    = 0;
			 this_action.charArray   = 0;
	*/
}

const action_t *agent_start(const observation_t *this_observation) {

	cerr << "start agent_start" << endl;

	/* This agent always returns a random number, either 0 or 1 for its action */
	int theIntAction=randInRange(1);
	this_action.intArray[0]=theIntAction;

	/* In a real action you might want to store the last observation and last action*/
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);

	cerr << "done agent_start" << endl;
	
	return &this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {


	cerr << "start agent_step" << endl;

	/* This agent  returns 0 or 1 randomly for its action */
	int theIntAction=randInRange(1);
	this_action.intArray[0]=theIntAction;
	
	
	/* In a real action you might want to store the last observation and last action*/
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);

	cerr << "done agent_step" << endl;
	
	return &this_action;
}

void agent_end(double reward) {
	clearRLStruct(&last_action);
	clearRLStruct(last_observation);
}

void agent_cleanup() {
	clearRLStruct(&this_action);
	clearRLStruct(&last_action);
	freeRLStructPointer(last_observation);
}

const char* agent_message(const char* inMessage) {
	if(strcmp(inMessage,"what is your name?")==0)
		return "my name is skeleton_agent!";

	return "I don't know how to respond to your message";
}
