#include <fstream>
#include <istream>
#include <sstream>

#include "RLAgent.h"



void RLAgent::load(Scene *scene, const string &filename) {

	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

  ifstream fin(filename.c_str());

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);

	string creatureName;

	doc["creature"] >> creatureName;

	creature = scene->getCreature(creatureName);

	fin.close();

}


void RLAgent::step(const LifeSim::RLStateDesc &state, LifeSim::RLActionDesc &action) {

	// TODO : Refactor
	const float constraintMultiplier = 0.0f;
	const float constraintStrength = 0.0001;

	action.Clear();

	for (unsigned i=0; i<creature->hingeConstraints.size(); i++)
		action.add_action(-constraintMultiplier * creature->hingeConstraints[i]->getAngle());


	for (unsigned i=0; i<creature->universalConstraints.size(); i++) {
		action.add_action(-constraintMultiplier * creature->universalConstraints[i]->getAngle(0));
		action.add_action(-constraintMultiplier * creature->universalConstraints[i]->getAngle(1));
	}

	assert(action.action_size() == creature->hingeConstraints.size() + creature->universalConstraints.size() * 2);

}
