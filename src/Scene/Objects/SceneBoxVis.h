#ifndef SCENE_BOX_VIS_H
#define SCENE_BOX_VIS_H

#include "SceneBox.h"
#include "Mesh.h"
#include "VertexProgram.h"
#include "ResourceManager.h"

#include "SceneObjectVis.h"

#include "Material.h"

#include <math.h>
#include <iostream>

using namespace std;

class SceneBoxVis : public SceneObjectVis {

 private:
  SceneBox *box;
  Mesh mesh;
  VertexProgram *vertexProgram;
  FragmentProgram *fragmentProgram;
  Material *material;

  bool *lightFacing;

 public:

  SceneBoxVis(SceneBox *_box) : box(_box) {
    mesh.createBox(box->halfExtents);

    lightFacing = new bool[mesh.getNumIndices()/3];

    vertexProgram = ResourceManager::instance().loadVertexProgram("res/shaders/phong.vp");
    vertexProgram->retain();

    fragmentProgram = ResourceManager::instance().loadFragmentProgram("res/shaders/phong.fp");
    fragmentProgram->retain();


    material = new Material(Vector4f(0.05, 0.05, 0.05, 1.0),
			    Vector4f(1.0, 1.0, 1.0, 1.0),
			    Vector4f(0.8, 0.8, 0.8, 0.8),
			    20.0f);

  }

  ~SceneBoxVis() {

    if (lightFacing)
      delete[] lightFacing;

    if (material)
      delete material;

    if (fragmentProgram)
      fragmentProgram->release();

    if (vertexProgram)
      vertexProgram->release();

  }

	SceneBox* getBox() {
		return box;
	}

  void render(bool ambientPass);
	void renderTransform();
  void renderLightCap();
  void renderDarkCap(const Vector4f &lightPosition);
  void renderShadowSides(const Vector4f &lightPosition);


  void calcLightFacing(const Vector4f &lightPosition);

};

#endif
