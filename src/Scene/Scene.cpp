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

	for (vector<ScenePlane*>::iterator it = planes.begin(); it != planes.end(); it++) {
		scenePhysics->removeSceneRigidBody(*it);
		delete *it;
	}
	planes.clear();

	for (vector<SceneSphere*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
		scenePhysics->removeSceneRigidBody(*it);
		delete *it;
	}
	spheres.clear();

	for (vector<SceneBox*>::iterator it = boxes.begin(); it != boxes.end(); it++) {
		scenePhysics->removeSceneRigidBody(*it);
		delete *it;
	}
	boxes.clear();

	for (vector<Creature*>::iterator it = creatures.begin(); it != creatures.end(); it++) {
		(*it)->removeFromScenePhysics(scenePhysics);
		delete *it;
	}
	creatures.clear();


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

				ScenePlane *scenePlane = new ScenePlane(name, plane);
				planes.push_back(scenePlane);

				scenePhysics->addSceneRigidBody(scenePlane);

				if (sceneObserver)
					sceneObserver->onSceneAddPlane(scenePlane, origin, size);
			}

			else if (sceneObjectDesc.type() == SceneObjectDesc_Type_BOX) {

				const SceneBoxDesc &boxDesc = sceneObjectDesc.GetExtension(SceneBoxDesc::scene_object);

				Transform transform(boxDesc.transform());
				Vector3f halfExtents(boxDesc.half_extents());

				SceneBox *sceneBox = new SceneBox(name, halfExtents, transform);
				boxes.push_back(sceneBox);

				scenePhysics->addSceneRigidBody(sceneBox);

				if (sceneObserver)
					sceneObserver->onSceneAddBox(sceneBox);

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



		// TODO : Refactor!!!  We're assuming a config based scene object is a creature
		if (ref.has_config()) {

			const string &config = ref.config();

			Creature *creature = new Creature();
			creature->load(dir + "/" + config);

			creatures.push_back(creature);

			creature->addToScenePhysics(scenePhysics);

			if (sceneObserver)
				sceneObserver->onSceneAddCreature(creature);

		}


	}





	/*
  for(unsigned i=0;i<doc.size();i++) {

    //		cout << "blah" << endl;

    string type;

    doc[i]["type"] >> type;

		if (type == "plane") {
			Vector3f origin;
			Planef plane;
			float size;
			string name;

			doc[i]["name"] >> name;
			doc[i]["origin"] >> origin;
			doc[i]["plane"] >> plane;
			doc[i]["size"] >> size;

			ScenePlane *scenePlane = new ScenePlane(name, plane);
			planes.push_back(scenePlane);

			scenePhysics->addSceneRigidBody(scenePlane);

			//			cout << scenePlane->plane << endl;

			if (sceneObserver)
				sceneObserver->onSceneAddPlane(scenePlane, origin, size);

		}


		else if (type == "sphere") {
			Vector3f origin;
			float radius;
			string name;

			doc[i]["name"] >> name;
			doc[i]["origin"] >> origin;
			doc[i]["radius"] >> radius;

			SceneSphere *sceneSphere = new SceneSphere(name, radius, origin);
			spheres.push_back(sceneSphere);

			scenePhysics->addSceneRigidBody(sceneSphere);

			if (sceneObserver)
				sceneObserver->onSceneAddSphere(sceneSphere);

		}

		else if (type == "box") {
			Vector3f position;
			Vector3f halfExtents;
			Quaternionf rotation;
			string name;

			doc[i]["name"] >> name;
			doc[i]["position"] >> position;
			doc[i]["rotation"] >> rotation;
			doc[i]["halfExtents"] >> halfExtents;

			SceneBox *sceneBox = new SceneBox(name, halfExtents, Transform(position, rotation));
			boxes.push_back(sceneBox);

			scenePhysics->addSceneRigidBody(sceneBox);

			if (sceneObserver)
				sceneObserver->onSceneAddBox(sceneBox);

		}

		else if (type == "light") {
			Vector4f position, diffuse, specular, ambient;
			string name;

			doc[i]["name"] >> name;
			doc[i]["position"] >> position;
			doc[i]["diffuse"] >> diffuse;
			doc[i]["specular"] >> specular;
			doc[i]["ambient"] >> ambient;

			if (sceneObserver)
				sceneObserver->onSceneAddLight(position, diffuse, specular, ambient);

		}

		else if (type == "creature") {
			string name, config;

			doc[i]["name"] >> name;
			doc[i]["config"] >> config;

			Creature *creature = new Creature(name);
			creature->load(dir + "/" + config);

			creatures.push_back(creature);

			creature->addToScenePhysics(scenePhysics);

			if (sceneObserver)
				sceneObserver->onSceneAddCreature(creature);

		}

  }

	fin.close();
  */


	return true;

}


void Scene::step(const float step) {

	scenePhysics->step(step);

}


Creature* Scene::getCreature(const string &name) {

	for (unsigned i=0; i<creatures.size(); i++)
		if (creatures[i]->getName() == name)
			return creatures[i];

	return NULL;

}

