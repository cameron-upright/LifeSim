#ifndef SCENE_PLANE_VIS
#define SCENE_PLANE_VIS

#include "ScenePlane.h"

#include <math.h>
#include <iostream>

#include "Material.h"
#include "VertexProgram.h"
#include "ResourceManager.h"

using namespace std;

class ScenePlaneVis {

 private:
  ScenePlane *plane;
  Material *material;
  Vector3f origin;
  float scale;

  VertexProgram *vertexProgram;
  FragmentProgram *fragmentProgram;


 public:

  ScenePlaneVis(ScenePlane *_plane, const float _scale = 5.0f, const Vector3f &_origin = Vector3f()) : plane(_plane) {

    scale = _scale;
    origin = plane->plane.project(_origin);

    material = new Material(Vector4f(0.05, 0.05, 0.15, 1.0),
														Vector4f(0.2,  0.2,  0.4,  1.0),
														Vector4f(0.2,  0.2,  0.4,  1.0),
														128.0f);

    vertexProgram = ResourceManager::instance().loadVertexProgram("res/shaders/phong.vp");
    vertexProgram->retain();

    fragmentProgram = ResourceManager::instance().loadFragmentProgram("res/shaders/phong.fp");
    fragmentProgram->retain();

  }

  ~ScenePlaneVis() {

    if (material)
      delete material;

    if (fragmentProgram)
      fragmentProgram->release();

    if (vertexProgram)
      vertexProgram->release();

  }

  void render(bool ambientPass);


 private :

  void applyTransform() {

    // rotation from the vertical, around the x axis
    float phi = acos(plane->plane.v * Vector3f(0,1,0)) * 180.0f / M_PI;

    // rotation around the y axis
    float theta = atan2(plane->plane.v[0], plane->plane.v[2]) * 180.0f / M_PI;

    //    cout << phi << " " << theta << endl;

    glTranslatef(origin[0], origin[1], origin[2]);
    glRotatef(theta, 0.0, 1.0, 0.0);
    glRotatef(phi, 1.0, 0.0, 0.0);

  }


};

#endif
