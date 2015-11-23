#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "Scene.h"


using namespace std;

using namespace LifeSim;

Scene::Scene() {

	sceneObserver = NULL;
	scenePhysics = new ScenePhysics();

}

Scene::Scene(const char *filename) {

	sceneObserver = NULL;
  load(filename);
	scenePhysics = new ScenePhysics();

}

Scene::~Scene() {

	for (auto &objIt : objectMap) {

		SceneRigidBodyObject *rigidBodyObj = dynamic_cast<SceneRigidBodyObject*>(objIt.second.get());

		if (rigidBodyObj)
			scenePhysics->removeSceneRigidBody(rigidBodyObj);

		Creature *creatureObj = dynamic_cast<Creature*>(objIt.second.get());
		if (creatureObj)
			creatureObj->removeFromScenePhysics(scenePhysics);
			


	}
	objectMap.clear();


	delete scenePhysics;

}

bool Scene::load(const char *filename) {


	// Get the directory of the file
	string filenameStr = string(filename);
	size_t found;
	found = filenameStr.find_last_of("/");
	string dir = filenameStr.substr(0,found);


	SceneDesc desc;

  int fd = open(filename, O_RDONLY);

  google::protobuf::io::FileInputStream fileInput(fd);
	google::protobuf::TextFormat::Parse(&fileInput, &desc);

	close(fd);

	
	
	for (int i=0; i<desc.scene_object_size(); i++) {

		SceneObjectRef ref = desc.scene_object(i);

		if (ref.has_desc()) {

			SceneObjectDesc sceneObjectDesc = ref.desc();

			string name = sceneObjectDesc.name();


			if (sceneObjectDesc.type() == SceneObjectDesc_Type_PLANE) {

				const ScenePlaneDesc &planeDesc = sceneObjectDesc.GetExtension(ScenePlaneDesc::scene_object);

				Vector3f origin(planeDesc.origin());
				Planef plane = planeDesc.plane();
				float size = planeDesc.size();

				std::shared_ptr<ScenePlane> scenePlane(new ScenePlane(name, plane));
				objectMap[name] = scenePlane;

				scenePhysics->addSceneRigidBody(scenePlane.get());

				if (sceneObserver)
					sceneObserver->onSceneAddPlane(scenePlane.get(), origin, size);

			}

			else if (sceneObjectDesc.type() == SceneObjectDesc_Type_BOX) {

				const SceneBoxDesc &boxDesc = sceneObjectDesc.GetExtension(SceneBoxDesc::scene_object);

				Transform transform(boxDesc.transform());
				Vector3f halfExtents(boxDesc.half_extents());

				std::shared_ptr<SceneBox> sceneBox(new SceneBox(name, halfExtents, transform));
				objectMap[name] = sceneBox;

				scenePhysics->addSceneRigidBody(sceneBox.get());

				if (sceneObserver)
					sceneObserver->onSceneAddBox(sceneBox.get());

			}

			else if (sceneObjectDesc.type() == SceneObjectDesc_Type_LIGHT) {

				const SceneLightDesc &lightDesc = sceneObjectDesc.GetExtension(SceneLightDesc::scene_object);

				Vector4f position(lightDesc.position());
				Vector4f ambient(lightDesc.ambient());
				Vector4f diffuse(lightDesc.diffuse());
				Vector4f specular(lightDesc.specular());

				if (sceneObserver)
					sceneObserver->onSceneAddLight(position, diffuse, specular, ambient);

			}

		}

		// TODO : Refactor!!!  We're assuming a config file based scene object is a creature
		if (ref.has_config()) {

			const string &config = ref.config();

			std::shared_ptr<Creature> creature(new Creature());
			creature->load(dir + "/" + config);
			objectMap[creature->name] = creature;

			creature->addToScenePhysics(scenePhysics);

			if (sceneObserver)
				sceneObserver->onSceneAddCreature(creature.get());

		}

	}


	return true;

}


/*
ScenePlane* Scene::resetOrCreatePlane(const string &name, const Planef &plane) {

	ScenePlane* plane = dynamic_cast<ScenePlane*>(objectMap[name]);

	if (plane != NULL) {
		

	}

	else {
		plane = new ScenePlane(name, plane);

	return plane;
		
}


SceneSphere* Scene::resetOrCreateSphere(const string &name) {

}


SceneBox* Scene::resetOrCreateBox(const string &name) {

}


Creature* Scene::resetOrCreateCreature(const string &name) {

}
*/




void Scene::step(const float step) {

	scenePhysics->step(step);

}


Creature* Scene::getCreature(const string &name) {

	auto it = objectMap.find(name);

	if (it != objectMap.end())
		return dynamic_cast<Creature*>(it->second.get());

	return NULL;

}

