#include <fstream>
#include <istream>
#include <sstream>

#include "RLEnvironment.h"


RLEnvironment::RLEnvironment() {
  scene = new Scene();
}

RLEnvironment::~RLEnvironment() {
  delete scene;
}


void RLEnvironment::load(const string &filename) {

	// Get the directory of the file
	string filenameStr = string(filename);
  size_t found;
  found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);

  ifstream fin(filename.c_str());

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);

	string sceneConfig;
	string creatureName;

	doc["scene"] >> sceneConfig;
	doc["creature"] >> creatureName;

	scene->load((dir + "/" + sceneConfig).c_str());

	creature = scene->getCreature(creatureName);

	fin.close();

}

void RLEnvironment::start(RLState &state) {

	

}

void RLEnvironment::step(RLState &state, const RLAction &action, float &reward, float dt) {

	static float t = 0.0f;

	t += dt;

	applyControl(action);

	//	if (t < 0.03f)
	//		creature->rigidBodyMap["spine_2"]->getRigidBody()->applyCentralImpulse(btVector3(0,5.0,0) * dt);

	while (t > 2.5f)
		t -= 2.5;
		

  scene->step(dt);

}




void RLEnvironment::applyControl(const RLAction &action) {



  for (vector<SceneHingeConstraint*>::iterator it = creature->hingeConstraints.begin();
       it != creature->hingeConstraints.end();
       it++) {

		const RLConstraintAction *constraintAction = action.getConstraintAction((*it)->name);

    (*it)->enableAngularMotor(true, constraintAction->targetVelocity[0], constraintAction->maxMotorImpulse[0]);

  }



  for (vector<SceneUniversalConstraint*>::iterator it = creature->universalConstraints.begin(); 
       it != creature->universalConstraints.end(); 
       it++) {

		const RLConstraintAction *constraintAction = action.getConstraintAction((*it)->name);

    (*it)->enableAngularMotor(0, true, constraintAction->targetVelocity[0], constraintAction->maxMotorImpulse[0]);
		(*it)->enableAngularMotor(1, true, constraintAction->targetVelocity[1], constraintAction->maxMotorImpulse[1]);

  }

}

