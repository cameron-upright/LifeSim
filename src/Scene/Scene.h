#ifndef SCENE_H
#define SCENE_H

#include <istream>

#include <memory>

#include "SceneObserver.h"

#include "ScenePlane.h"
#include "SceneSphere.h"
#include "SceneBox.h"

#include "Creature.h"

#include "ScenePhysics.h"

using namespace std;

class Scene {

  SceneObserver *sceneObserver;
  ScenePhysics *scenePhysics;

	map<string, std::shared_ptr<SceneObject> > objectMap;

	string creatureNameHack;

public:

  Scene(void);
  Scene(const char *filename);

	Scene(const Scene &scene) = delete;
	Scene(Scene &&scene) = delete;
  ~Scene();

	Scene& operator=(const Scene &other) = delete;
	Scene& operator=(Scene &&other) = delete;

	void destroyScenePhysics();
  bool load(const char *filename);

  void setObserver(SceneObserver *sceneObserver) {
    this->sceneObserver = sceneObserver;
  }

  void step(const float step);

  // TODO : Fix this to work with any SceneObject, since creatures and everything else is a SceneObject
  Creature* getCreature(const string &name);


};

#endif
