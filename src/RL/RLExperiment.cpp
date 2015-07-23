#include <fstream>
#include <istream>
#include <sstream>

#include <assert.h>

#include "RLExperiment.h"

RLExperiment::RLExperiment(RLEnvironment *env, RLAgent *agent, const RLExperimentDescription &desc) : env(env), agent(agent), desc(desc) {

	isEpisodeStart = true;

}

RLExperiment::~RLExperiment() {
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


