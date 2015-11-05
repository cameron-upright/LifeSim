#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <string>

using namespace std;

class SceneObject {

public:

  string name;

	SceneObject() {}
  SceneObject(string name_) {
    name = name_;
  }

	virtual ~SceneObject() {}

  const string getName() const {
    return name;
  }

};


#endif //SCENE_OBJECT_H
