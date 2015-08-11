#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "RLEnvironment.h"

#include "LifeSim.pb.h"


using namespace LifeSim;


RLEnvironment::RLEnvironment() {
  scene = new Scene();
}

RLEnvironment::~RLEnvironment() {
  delete scene;
}


void RLEnvironment::load(const string &filename) {


  RLEnvironmentDesc desc;

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

}

void RLEnvironment::start(RLStateDesc &state) {

	

}

void RLEnvironment::step(RLStateDesc &state, const RLAction &action, float &reward, float dt) {

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

