#ifndef SCENE_CONSTRAINT_H
#define SCENE_CONSTRAINT_H

#include <string>

using namespace std;

enum SceneConstraintType {
  SceneHingeConstraintType,
  SceneUniversalConstraintType
};


class SceneConstraint {

public:

  string name;

  SceneConstraint(string name) : name(name) {}

  virtual btTypedConstraint *getConstraint() = 0;
  virtual SceneConstraintType getConstraintType() = 0;

};


#endif // SCENE_CONSTRAINT_H
