#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "LifeSim.pb.h"

#include "Creature.h"

#include "Vector2.h"

using namespace LifeSim;

Creature::Creature(const string &name_) : SceneObject(name_) {
}

Creature::~Creature() {

  for (vector<SceneBox*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
    rigidBodyMap.erase((*it)->getName().c_str());
    delete *it;
  }

  for (vector<SceneHingeConstraint*>::iterator it = hingeConstraints.begin(); it != hingeConstraints.end(); it++) {
    constraintMap.erase((*it)->name);
    delete *it;
  }

  for (vector<SceneUniversalConstraint*>::iterator it = universalConstraints.begin(); it != universalConstraints.end(); it++) {
    constraintMap.erase((*it)->name);
    delete *it;
  }

}

bool Creature::load(const string &filename) {

	SceneCreatureDesc desc;

  int fd = open(filename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);

	for (int i=0; i<desc.child_scene_object_size(); i++) {

		SceneObjectDesc bodyDesc = desc.child_scene_object(i);

    string name = bodyDesc.name();
		//		rotation.normalize();

		//		cout << name << " " << position << " " << rotation << endl;

		if (bodyDesc.type() == SceneObjectDesc_Type_BOX) {

			const SceneBoxDesc &boxDesc = bodyDesc.GetExtension(SceneBoxDesc::scene_object);

			Transform transform(boxDesc.transform());
			Vector3f halfExtents(boxDesc.half_extents());


			SceneBox *sceneBox = new SceneBox(name, halfExtents, transform);

			boxes.push_back(sceneBox);

			rigidBodyMap[sceneBox->getName().c_str()] = sceneBox;

		}

	}






	for (int i=0; i<desc.constraint_size(); i++) {

		ConstraintDesc constraintDesc = desc.constraint(i);

    string name = constraintDesc.name();

		string bodyNameA = constraintDesc.body(0);
		string bodyNameB = constraintDesc.body(1);

		if (constraintDesc.type() == ConstraintDesc_Type_HINGE) {

			const HingeConstraintDesc &hingeDesc = constraintDesc.GetExtension(HingeConstraintDesc::constraint);

			Vector3f axisA  = hingeDesc.axis_in_a();
			Vector3f axisB  = hingeDesc.axis_in_b();
			Vector3f pivotA = hingeDesc.pivot_in_a();
			Vector3f pivotB = hingeDesc.pivot_in_b();
			Vector2f limit  = hingeDesc.limit();

			SceneRigidBodyObject *bodyA, *bodyB;

			//    cout << "\"" << bodyNameA << "\"" << endl;

			bodyA = rigidBodyMap[bodyNameA.c_str()];
			bodyB = rigidBodyMap[bodyNameB.c_str()];

			SceneHingeConstraint *sceneHingeConstraint = new SceneHingeConstraint(btVector3(axisA[0], axisA[1], axisA[2]),
																																						btVector3(axisB[0], axisB[1], axisB[2]),
																																						btVector3(pivotA[0], pivotA[1], pivotA[2]),
																																						btVector3(pivotB[0], pivotB[1], pivotB[2]),
																																						limit, bodyA, bodyB, name);
			hingeConstraints.push_back(sceneHingeConstraint);
			constraintMap[name] = sceneHingeConstraint;

			/*
				cout << axisA << endl;
				cout << axisB << endl;
				cout << pivotA << endl;
				cout << pivotB << endl;
				cout << bodyA << endl;
				cout << bodyB << endl;
				cout << endl;
			*/

		} else if (constraintDesc.type() == ConstraintDesc_Type_UNIVERSAL) {

			const UniversalConstraintDesc &universalDesc = constraintDesc.GetExtension(UniversalConstraintDesc::constraint);

			Vector3f pivot = universalDesc.pivot();
			Vector3f axis0 = universalDesc.axis_0();
			Vector3f axis1 = universalDesc.axis_1();
			Vector2f limit0 = universalDesc.limit_0();
			Vector2f limit1 = universalDesc.limit_1();

			SceneRigidBodyObject *bodyA, *bodyB;

			//    cout << "\"" << bodyNameA << "\"" << endl;

			bodyA = rigidBodyMap[bodyNameA.c_str()];
			bodyB = rigidBodyMap[bodyNameB.c_str()];

			SceneUniversalConstraint *sceneUniversalConstraint = new SceneUniversalConstraint(btVector3(axis0[0], axis0[1], axis0[2]),
																																												btVector3(axis1[0], axis1[1], axis1[2]),
																																												btVector3(pivot[0], pivot[1], pivot[2]),
																																												limit0, limit1,
																																												bodyA, bodyB, name);
			universalConstraints.push_back(sceneUniversalConstraint);
			constraintMap[name] = sceneUniversalConstraint;

			/*
				cout << axis0 << endl;
				cout << axis1 << endl;
				cout << pivot << endl;
				cout << bodyA << endl;
				cout << bodyB << endl;
				cout << limit0 << endl;
				cout << limit1 << endl;
				cout << endl;
			*/


		}
	}



	/*
  ifstream fin(filename.c_str());

  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);

  string creatureName;
  doc["creature_name"] >> creatureName;

  const YAML::Node& rigid_bodies = doc["rigid_bodies"];

  for(unsigned i=0;i<rigid_bodies.size();i++)
    parseRigidBody(rigid_bodies[i]);

  const YAML::Node* constraints = doc.FindValue("constraints");

  
  if (constraints)
    for(unsigned i=0;i<constraints->size();i++)
      parseConstraint((*constraints)[i]);

  return true;
	*/

}



void Creature::addToScenePhysics(ScenePhysics *scenePhysics) {

  for (vector<SceneBox*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
    scenePhysics->addSceneRigidBody(*it);
  }

  for (vector<SceneHingeConstraint*>::iterator it = hingeConstraints.begin(); it != hingeConstraints.end(); it++)
    scenePhysics->addSceneConstraint(*it);

  for (vector<SceneUniversalConstraint*>::iterator it = universalConstraints.begin(); it != universalConstraints.end(); it++)
    scenePhysics->addSceneConstraint(*it);

}


void Creature::removeFromScenePhysics(ScenePhysics *scenePhysics) {

  for (vector<SceneBox*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
    scenePhysics->removeSceneRigidBody(*it);
  }

  for (vector<SceneHingeConstraint*>::iterator it = hingeConstraints.begin(); it != hingeConstraints.end(); it++)
    scenePhysics->removeSceneConstraint(*it);

  for (vector<SceneUniversalConstraint*>::iterator it = universalConstraints.begin(); it != universalConstraints.end(); it++)
    scenePhysics->removeSceneConstraint(*it);


}

