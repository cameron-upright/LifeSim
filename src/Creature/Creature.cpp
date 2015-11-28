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

Creature::~Creature() {}

bool Creature::load(const string &filename) {

	SceneObjectDesc desc;


  int fd = open(filename.c_str(), O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);

	assert(desc.type() == SceneObjectDesc_Type_CREATURE);

	const SceneCreatureDesc &creatureDesc = desc.GetExtension(SceneCreatureDesc::scene_object);

	this->name = desc.name();


	// TODO : ugh
	originalRotation.clear();

	for (int i=0; i<creatureDesc.child_scene_object_size(); i++) {

		SceneObjectDesc bodyDesc = creatureDesc.child_scene_object(i);

    string name = bodyDesc.name();
		//		rotation.normalize();

		if (bodyDesc.type() == SceneObjectDesc_Type_BOX) {


			const SceneBoxDesc &boxDesc = bodyDesc.GetExtension(SceneBoxDesc::scene_object);

			Transform transform(boxDesc.transform());
			Vector3f halfExtents(boxDesc.half_extents());

			auto objIt = rigidBodyMap.find(name);

			SceneBox* sceneBox;

			if (objIt != rigidBodyMap.end()) {

				//				cout << "RESET " << name << endl;

				sceneBox = dynamic_cast<SceneBox*>(objIt->second.get());
				sceneBox->reset(halfExtents, transform);

			} else {

				sceneBox = new SceneBox(name, halfExtents, transform);
				rigidBodyMap[name] = std::shared_ptr<SceneBox>(sceneBox);

			}

			btTransform currentTransform;
			sceneBox->getMotionState()->getWorldTransform(currentTransform);
			originalRotation[name] = currentTransform.getRotation();

		}

	}



	// Clear out the old constraints
	// TODO : Ugh, nasty

	
	hingeConstraints.clear();
	universalConstraints.clear();
	constraintMap.clear();


	for (int i=0; i<creatureDesc.constraint_size(); i++) {

		ConstraintDesc constraintDesc = creatureDesc.constraint(i);

    string name = constraintDesc.name();

		if (constraintMap.count(name))
			continue;

		string bodyNameA = constraintDesc.body(0);
		string bodyNameB = constraintDesc.body(1);

		if (constraintDesc.type() == ConstraintDesc_Type_HINGE) {

			const HingeConstraintDesc &hingeDesc = constraintDesc.GetExtension(HingeConstraintDesc::constraint);

			Vector3f axisA(hingeDesc.axis_in_a());
			Vector3f axisB(hingeDesc.axis_in_b());
			Vector3f pivotA(hingeDesc.pivot_in_a());
			Vector3f pivotB(hingeDesc.pivot_in_b());
			Vector2f limit(hingeDesc.limit());

			shared_ptr<SceneRigidBodyObject> bodyA, bodyB;

			//    cout << "\"" << bodyNameA << "\"" << endl;

			bodyA = rigidBodyMap[bodyNameA.c_str()];
			bodyB = rigidBodyMap[bodyNameB.c_str()];

			std::shared_ptr<SceneHingeConstraint>
				sceneHingeConstraint(new SceneHingeConstraint(btVector3(axisA[0], axisA[1], axisA[2]),
																											btVector3(axisB[0], axisB[1], axisB[2]),
																											btVector3(pivotA[0], pivotA[1], pivotA[2]),
																											btVector3(pivotB[0], pivotB[1], pivotB[2]),
																											limit, bodyA.get(), bodyB.get(), name));

			hingeConstraints.push_back(sceneHingeConstraint);
			constraintMap[name] = sceneHingeConstraint;

		} else if (constraintDesc.type() == ConstraintDesc_Type_UNIVERSAL) {

			const UniversalConstraintDesc &universalDesc = constraintDesc.GetExtension(UniversalConstraintDesc::constraint);

			Vector3f pivot(universalDesc.pivot());
			Vector3f axis0(universalDesc.axis_0());
			Vector3f axis1(universalDesc.axis_1());
			Vector2f limit0(universalDesc.limit_0());
			Vector2f limit1(universalDesc.limit_1());

			shared_ptr<SceneRigidBodyObject> bodyA, bodyB;

			//    cout << "\"" << bodyNameA << "\"" << endl;

			bodyA = rigidBodyMap[bodyNameA.c_str()];
			bodyB = rigidBodyMap[bodyNameB.c_str()];

			std::shared_ptr<SceneUniversalConstraint>
				sceneUniversalConstraint(new SceneUniversalConstraint(btVector3(axis0[0], axis0[1], axis0[2]),
																															btVector3(axis1[0], axis1[1], axis1[2]),
																															btVector3(pivot[0], pivot[1], pivot[2]),
																															limit0, limit1,
																															bodyA.get(), bodyB.get(), name));
			
			universalConstraints.push_back(sceneUniversalConstraint);
			constraintMap[name] = std::shared_ptr<SceneConstraint>(sceneUniversalConstraint);
			
		}
	}

	return true;

}



void Creature::addToScenePhysics(ScenePhysics *scenePhysics) {

	for (auto &it : rigidBodyMap)
    scenePhysics->addSceneRigidBody(it.second.get());

  for (auto &constraint : hingeConstraints)
    scenePhysics->addSceneConstraint(constraint.get());

  for (auto &constraint : universalConstraints)
    scenePhysics->addSceneConstraint(constraint.get());

}


void Creature::removeFromScenePhysics(ScenePhysics *scenePhysics) {

  for (auto &constraint : hingeConstraints)
    scenePhysics->removeSceneConstraint(constraint.get());

  for (auto &constraint : universalConstraints)
    scenePhysics->removeSceneConstraint(constraint.get());

	for (auto &it : rigidBodyMap)
    scenePhysics->removeSceneRigidBody(it.second.get());

}

