#ifndef CREATURE_H
#define CREATURE_H

#include "ScenePhysics.h"

#include <string>
#include <vector>
#include <ext/hash_map>

#include "SceneObject.h"
#include "SceneBox.h"

#include "SceneHingeConstraint.h"
#include "SceneUniversalConstraint.h"

using namespace std;
using namespace __gnu_cxx;


struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

class Scene;

class Creature : public SceneObject {

public:

  vector<SceneBox*> boxes;
  vector<SceneHingeConstraint*> hingeConstraints;
  vector<SceneUniversalConstraint*> universalConstraints;

  map<string, SceneConstraint*> constraintMap;
  hash_map<const char *, SceneRigidBodyObject*, hash<const char *>, eqstr> rigidBodyMap;


  Creature(const string &name_);
  ~Creature();

  bool load(const string &filename);


  void parseRigidBody(const YAML::Node& node);
  void parseConstraint(const YAML::Node& node);

  void addToScenePhysics(ScenePhysics *scenePhysics);
  void removeFromScenePhysics(ScenePhysics *scenePhysics);

};


#endif
