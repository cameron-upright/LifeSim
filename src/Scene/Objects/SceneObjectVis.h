#ifndef SCENE_OBJECT_VIS_H
#define SCENE_OBJECT_VIS_H

#include "Vector4.h"

class SceneObjectVis {

 public:

  SceneObjectVis() {
  }

  virtual ~SceneObjectVis() {}

  virtual void render(bool ambientPass) = 0;
  virtual void renderLightCap() = 0;
  virtual void renderDarkCap(const Vector4f &lightPosition) = 0;
  virtual void renderShadowSides(const Vector4f &lightPosition) = 0;

  virtual void calcLightFacing(const Vector4f &lightPosition) = 0;

};

#endif
