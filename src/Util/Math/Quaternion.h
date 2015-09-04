#ifndef QUATERNION_H
#define QUATERNION_H

#include <assert.h>
#include "Vector3.h"

#include <LinearMath/btVector3.h>


template <typename T>
class Quaternion {

 public:

  Vector3<T> v;
  T w;

	// Identity
  Quaternion(void) : v(0,0,0), w(1) {}

  Quaternion(const Quaternion<T> &q) {
    v = q.v;
    w = q.w;
  }

  explicit Quaternion(const Vector3<T> &v2) {
    v = v2;
    w = 0.0;
  }

  Quaternion(const Vector3<T> &v2, const T &w2) {
    v = v2;
    w = w2;
  }

  Quaternion(const T *v2) {
    v[0] = v2[0];
    v[1] = v2[1];
    v[2] = v2[2];
    w = v2[3];
  }

  Quaternion(const T &q0, const T &q1, const T &q2, const T &q3) {
    v[0] = q0;
    v[1] = q1;
    v[2] = q2;
    w = q3;
  }

  explicit Quaternion(const btQuaternion &q) {

    v[0] = q.getX();
    v[1] = q.getY();
    v[2] = q.getZ();
    w    = q.getW();

  }

	explicit Quaternion(const LifeSim::VectorDesc &desc) {
		assert(desc.x_size() == 4);
		v[0] = desc.x(0);
		v[1] = desc.x(1);
		v[2] = desc.x(2);
		w    = desc.x(3);
	}


  T lengthSquared(void) {
    return w*w + v*v;
  }

  T length(void) {
    return sqrt(lengthSquared());
  }


  Quaternion<T> inverse(void) const {
    return Quaternion<T>(v*-1.0, w);
  }


  void setRotation(const Vector3<T> &axis, const T &angle) {
    w = cos(angle/2.0);
    v = axis * sin(angle/2.0);
  }

  void normalize(void) {
    T n;

    n = 1.0/length();

    w *= n;
    v = v * n;
  }

  /*
  void get_matrix(Matrix3x3<T> &M) {

    // 1 - 2*qy2 - 2*qz2  	2*qx*qy - 2*qz*qw  	2*qx*qz + 2*qy*qw
    // 2*qx*qy + 2*qz*qw 	1 - 2*qx2 - 2*qz2 	2*qy*qz - 2*qx*qw
    // 2*qx*qz - 2*qy*qw 	2*qy*qz + 2*qx*qw 	1 - 2*qx2 - 2*qy2

    M.m[0] = 1 - 2*v[1]*v[1] - 2*v[2]*v[2];
    M.m[1] = 2*v[0]*v[1] + 2*v[2]*w;
    M.m[2] = 2*v[0]*v[2] - 2*v[1]*w;
    M.m[3] = 2*v[0]*v[1] - 2*v[2]*w;
    M.m[4] = 1 - 2*v[0]*v[0] - 2*v[2]*v[2];
    M.m[5] = 2*v[1]*v[2] + 2*v[0]*w;
    M.m[6] = 2*v[0]*v[2] + 2*v[1]*w;
    M.m[7] = 2*v[1]*v[2] - 2*v[0]*w;
    M.m[8] = 1 - 2*v[0]*v[0] - 2*v[1]*v[1];

  }
  */


  // Not sure what convention this used
  /*
  void get_euler_angles(Vector3<T> &vec) {
    vec[0] = atan2(2.0 * (w*v[0] + v[1]*v[2]), (1.0 - 2.0*(v[0]*v[0] + v[1]*v[1])));
    vec[1] = asin(min(1.0, max(-1.0, 2.0*(w*v[1] - v[0]*v[2]))));
    vec[2] = atan2(2.0*(w*v[2] + v[0]*v[1]), (1.0 - 2.0*(v[1]*v[1] + v[2]*v[2])));
  }
  */

  void operator= (const Quaternion<T> &q) {
    w = q.w;
    v = q.v;
  }

  Quaternion<T> operator* (const Quaternion<T> &q) const {
    Quaternion<T> q2;

    q2.w = w * q.w - (v * q.v);
    q2.v = q.v * w + v * q.w + cross(v, q.v);

    /*
    printf("%f %f %f %f %f %f\n", v[0], v[1], v[2], q.v[0], q.v[1], q.v[2]);
    printf("%f %f\n", w*q.w, -(v*q.v));
    */

    return q2;
  }

  /*
  Quaternion<T> operator* (const Vector3<T> &v2) {
    Quaternion<T> q2;

    q2.w = -(v * v2);
    q2.v = w * v2 + cross(v, v2);

    return q2;
  }
  */

  Vector3<T> operator* (const Vector3<T> &v2) const {

    return ((*this) * Quaternion<T>(v2, 0.0) * inverse()).v;

    /*
    Quaternion q2, p, inv;

    p = Quaternion<T>(v2, 0.0);

    inv = (*this).inverse();

    q2 = (*this) * p;

    q2 = q2 * inverse();


    return q2.v;
    */


  }


  void slerp(const Quaternion<T> &q0, const Quaternion<T> &q1, float t) {
    Quaternion<T> q2;
    float c0, c1;

    if (t < 0.0) {
      *this = q0;
      return;
    } else if (t > 1.0) {
      *this = q1;
      return;
    }

    float cos_val = q0.v * q1.v + q0.w * q1.w;

    if (cos_val < 0.0f) {
      q2.v[0] = -q1.v[0];
      q2.v[1] = -q1.v[1];
      q2.v[2] = -q1.v[2];
      q2.w = -q1.w;

      cos_val = -cos_val;

    } else
      q2 = q1;

    assert (cos_val < 1.001f);

    if (cos_val > 0.9999f) {
      c0 = 1.0f - t;
      c1 = t;
    } else {
      float sin_val = sqrt (1.0f - (cos_val * cos_val));
      float omega = atan2 (sin_val, cos_val);

      float sin_inv = 1.0f / sin_val;

      /* Compute interpolation parameters */
      c0 = sin ((1.0f - t) * omega) * sin_inv;
      c1 = sin (t * omega) * sin_inv;
    }

    v = q0.v * c0 + q2.v * c1;
    w = q0.w * c0 + q2.w * c1; 

  }

  operator const btQuaternion () const {
    return btQuaternion(v[0], v[1], v[2], w);
  }

};


template<class S>
std::ostream&
operator<<(std::ostream& os, const Quaternion<S>& q) {
  return os << q.v << " " << q.w;
}


typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;

#endif // QUATERNION_H
