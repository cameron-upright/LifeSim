#ifndef SCENE_VIS_H
#define SCENE_VIS_H

#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include "Scene.h"
#include "SceneObserver.h"

#include "ScenePlaneVis.h"
#include "SceneSphereVis.h"
#include "SceneBoxVis.h"

#include "SceneLight.h"
#include "CreatureVis.h"

#include "Vector3.h"


#include "Mesh.h"


class SceneVis : public SceneObserver {

  Scene &scene;

  vector <SceneLight*> lights;

  vector <ScenePlaneVis*> planes;
  vector <SceneSphereVis*> spheres;
  vector <SceneBoxVis*> boxes;

  vector <CreatureVis*> creatures;

  Mesh mesh;

	// TODO : Kind of a hack
	bool follow;

 public:

  Vector3f lookat;
  float phi;
  float theta;
  float dist;

  SceneVis(Scene &scene);
  ~SceneVis();

  void render(void);
  void renderAmbientPass(void);
  void renderStencilPass(int lightIndex);
  void renderPhongPass(int lightIndex);


  // SceneObserver methods
  void onSceneAddLight(const Vector4f &pos_,
		       const Vector4f &diffuse_,
		       const Vector4f &specular_,
		       const Vector4f &ambient_);


  void onSceneAddPlane(ScenePlane *plane, const Vector3f &origin, const float size);
  void onSceneAddSphere(SceneSphere *sphere);
  void onSceneAddBox(SceneBox *box);

  void onSceneAddCreature(Creature *creature);


	bool isFollowSet() const {
		return follow;
	}

	void toggleFollow() {
		follow = !follow;
	}

 private:
  void applyCameraTransform();
  void applyFog();


};

#endif
