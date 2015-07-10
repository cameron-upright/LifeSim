#include <fstream>
#include <istream>
#include <sstream>

#include "Scene.h"


using namespace std;

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

	ifstream fin(filename);


  YAML::Parser parser(fin);
  YAML::Node doc;

  parser.GetNextDocument(doc);
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

			/*
			cout << name << endl;
			cout << type << endl;
			cout << plane << endl;
			cout << origin << endl;
			cout << size << endl;
			cout << endl;
			*/
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

			/*
			cout << name << endl;
			cout << type << endl;
			cout << origin << endl;
			cout << radius << endl;
			cout << endl;
			*/
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

			/*
			cout << name << endl;
			cout << type << endl;
			cout << position << endl;
			cout << rotation << endl;
			cout << halfExtents << endl;
			cout << endl;
			*/

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

			/*
			cout << name << endl;
			cout << position << endl;
			cout << diffuse << endl;
			cout << specular << endl;
			cout << ambient << endl;
			cout << endl;
			*/

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

			/*
			cout << name << endl;
			cout << config << endl;
			cout << endl;
			*/

		}

  }

	fin.close();

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

