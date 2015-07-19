#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "Quaternion.h"

#include <LinearMath/btQuaternion.h>


class Transform {

public:

  Vector3f    position;
  Quaternionf rotation;

  Transform(const Vector3f &position_, const Quaternionf &rotation_) : position(position_), rotation(rotation_) {}

	Transform(const LifeSim::TransformDesc &desc) {
		position = Vector3f(desc.position());
		rotation = Quaternionf(desc.rotation());
	}


  operator const btTransform () const {
    return btTransform(rotation, position);
  }

};

#endif // TRANSFORM_H
