#include <fstream>
#include <istream>
#include <sstream>
#include <yaml-cpp/yaml.h>

#include "Creature.h"

#include "Vector2.h"

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

}



void Creature::parseRigidBody(const YAML::Node& node) {

  string type;

  node["type"] >> type;

  if (type == "box") {
    string name;
    Vector3f position;
    Vector3f halfExtents;
    Quaternionf rotation;

    node["name"] >> name;
    node["position"] >> position;
    node["rotation"] >> rotation;
    node["halfExtents"] >> halfExtents;

    rotation.normalize();

    SceneBox *sceneBox = new SceneBox(name, halfExtents, Transform(position, rotation));

    boxes.push_back(sceneBox);

    rigidBodyMap[sceneBox->getName().c_str()] = sceneBox;

    /*
    cout << name << endl;
    cout << position << endl;
    cout << halfExtents << endl;
    cout << endl;
    */

  }

}


void Creature::parseConstraint(const YAML::Node& node) {

  string type;

  node["type"] >> type;

  if (type == "hinge") {
    Vector3f axisA, axisB, pivotA, pivotB;
    string bodyNameA, bodyNameB;
    Vector2f limit;
    string name;

    node["name"] >> name;
    node["axisA"] >> axisA;
    node["axisB"] >> axisB;
    node["pivotA"] >> pivotA;
    node["pivotB"] >> pivotB;
    node["bodyA"] >> bodyNameA;
    node["bodyB"] >> bodyNameB;
    node["limit"] >> limit;

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

  } else if (type == "universal") {
    Vector3f axis0, axis1, pivot;
    Vector2f limit0, limit1;
    string bodyNameA, bodyNameB;
    string name;

    node["name"] >> name;
    node["axis0"] >> axis0;
    node["axis1"] >> axis1;
    node["pivot"] >> pivot;
    node["bodyA"] >> bodyNameA;
    node["bodyB"] >> bodyNameB;
    node["limit0"] >> limit0;
    node["limit1"] >> limit1;

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

