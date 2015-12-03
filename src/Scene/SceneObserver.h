#ifndef SCENE_OBSERVER_H
#define SCENE_OBSERVER_H

#include "SceneLight.h"

#include "ScenePlane.h"
#include "SceneSphere.h"
#include "SceneBox.h"

#include "Creature.h"

class SceneObserver {

 public:

  virtual void onSceneAddLight(const Vector4f &pos_,
			       const Vector4f &diffuse_,
			       const Vector4f &specular_,
			       const Vector4f &ambient_) = 0;

  virtual void onSceneAddPlane(ScenePlane *plane, const Vector3f &origin, const float size) = 0;
  virtual void onSceneAddSphere(SceneSphere *sphere) = 0;
  virtual void onSceneAddBox(SceneBox *box) = 0;

  virtual void onSceneAddCreature(Creature *creature) = 0;


};

#endif // SCENE_OBSERVER_H
