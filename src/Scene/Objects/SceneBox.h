#ifndef SCENE_BOX_H
#define SCENE_BOX_H

#include <btBulletDynamicsCommon.h>

#include "SceneRigidBodyObject.h"

#include "Transform.h"

class SceneBox : public SceneRigidBodyObject {

  // Bullet stuff
  btCollisionShape *boxShape;
  btDefaultMotionState* boxMotionState;
  btRigidBody* boxRigidBody;

public:

  Vector3f halfExtents;
  Transform transform;

  SceneBox(const string &name_, const Vector3f &halfExtents_, const Transform &transform_);
  ~SceneBox();

	virtual void reset(const Vector3f &halfExtents_, const Transform &transform_);
	virtual void destroy();

	void create(const Vector3f &halfExtents_, const Transform &transform_);

  btCollisionShape* getCollisionShape() {
    return boxShape;
  };

  btDefaultMotionState* getMotionState() {
    return boxMotionState;
  }

  btRigidBody* getRigidBody() {
    return boxRigidBody;
  }

};


#endif //SCENE_BOX_H
