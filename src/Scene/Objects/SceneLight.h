#ifndef SCENE_LIGHT_H
#define SCENE_LIGHT_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Vector4.h"

using namespace std;

class SceneLight {
 public:
  Vector4f pos;
  Vector4f diffuse;
  Vector4f specular;
  Vector4f ambient;

  SceneLight(const Vector4f &pos_,
	     const Vector4f &diffuse_,
	     const Vector4f &specular_,
	     const Vector4f &ambient_) : pos(pos_), diffuse(diffuse_), specular(specular_), ambient(ambient_) {
  }

  SceneLight() {}

  void applyLight(int lightIndex) {

    glEnable(GL_LIGHT0 + lightIndex);
    glLightfv(GL_LIGHT0 + lightIndex, GL_AMBIENT, ambient.get_ptr());
    glLightfv(GL_LIGHT0 + lightIndex, GL_DIFFUSE, diffuse.get_ptr());
    glLightfv(GL_LIGHT0 + lightIndex, GL_SPECULAR, specular.get_ptr());
    glLightfv(GL_LIGHT0 + lightIndex, GL_POSITION, pos.get_ptr());

  }

};



#endif // SCENE_LIGHT_H
