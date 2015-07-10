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


void RLAgent::step(const RLState &state, RLAction &action) {

	const float constraintMultiplier = 0.0f;
	const float constraintStrength = 0.0001;

	for (unsigned i=0; i<creature->hingeConstraints.size(); i++) {

		string constraintName = creature->hingeConstraints[i]->name;
		RLConstraintAction constraintAction(-constraintMultiplier * creature->hingeConstraints[i]->getAngle(), 0.0f, constraintStrength, 0.0f);

		action.constraintActions[constraintName] = constraintAction;

  }



	for (unsigned i=0; i<creature->universalConstraints.size(); i++) {

		string constraintName = creature->universalConstraints[i]->name;
		RLConstraintAction constraintAction(-constraintMultiplier * creature->universalConstraints[i]->getAngle(0),
																				-constraintMultiplier * creature->universalConstraints[i]->getAngle(1),
																				constraintStrength, constraintStrength);

		action.constraintActions[constraintName] = constraintAction;

  }

}
