#ifndef SCENE_MATERIAL_H
#define SCENE_MATERIAL_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Vector4.h"

using namespace std;

// TODO : Make this a resource, that I can load from file
class Material {
 public:

  Vector4f ambient;
  Vector4f diffuse;
  Vector4f specular;
  float    shininess;

  Material(const Vector4f &ambient,
								const Vector4f &diffuse,
								const Vector4f &specular,
								const float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
  }

  ~Material() {}

  void applyMaterial() {

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.get_ptr());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.get_ptr());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.get_ptr());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  }

};



#endif // SCENE_MATERIAL_H
