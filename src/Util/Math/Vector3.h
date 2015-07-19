#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>
#include <yaml-cpp/yaml.h>
#include <LinearMath/btQuaternion.h>

#include "LifeSim.pb.h"


template<typename T>
struct Vector3 {

private:

  T v[3];

public:

  Vector3() {v[0] = 0.0; v[1] = 0.0; v[2] = 0.0;}

  Vector3(const T &x, const T &y, const T &z) {
    set(x,y,z);
  }

  Vector3(const T *vec) {
    set(vec);
  }

  Vector3(const btVector3 &vec) {
    set(vec.getX(), vec.getY(), vec.getZ());
  }

	Vector3(const LifeSim::VectorDesc &desc) {
		assert(desc.x_size() == 3);
		v[0] = desc.x(0);
		v[1] = desc.x(1);
		v[2] = desc.x(2);
	}

  void set(const T *vec) {
    set(vec[0], vec[1], vec[2]);
  }

  void set(const T &x, const T &y, const T &z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
  }

  T* ptr(void) { return (T*)this; }

  T length() const {
    return sqrt(length_sqr());
  }

  T length_sqr() const {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  }

  T dot(const Vector3<float> &vec) const {
    return v[0]*vec.v[0] + v[1]*vec.v[1] + v[2]*vec.v[2];
  }

  void scale(const T &s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
  }

  Vector3<T> normalized() {
    Vector3<T> vector = Vector3<T>(*this);
    vector.normalize();
    return vector;
  }

  void normalize() {
    T d = sqrt( length_sqr() );
    v[0] /= d; v[1] /= d; v[2] /= d;
  }

  void rotateY(double d)
  {
    double c = cos(d);
    double s = sin(d);
    double nx =   v[0] * c + v[2] * s;
    double nz = - v[0] * s + v[2] * c;
    v[0] = nx;
    v[2] = nz;
  }

  T* get_ptr(void) { return (T*)this; }

  const T* get_ptr(void) const { return (T*)this; }



  Vector3<T>
  operator*(const T &d) const
  {
    return Vector3(v[0]*d, v[1]*d, v[2]*d);
  }


  Vector3<T>
  operator/(const T &d) const
  {
    return Vector3(v[0]/d, v[1]/d, v[2]/d);
  }

  Vector3<T>
  operator+(const Vector3<T>& vec)
  {
    return Vector3<T>(v[0]+vec[0], v[1]+vec[1], v[2]+vec[2]);
  }

  template <typename Q>
  Vector3<T>
  operator-(const Vector3<Q>& vec) const
  {
    return Vector3<T>(v[0]-vec[0], v[1]-vec[1], v[2]-vec[2]);
  }


  Vector3<T>
  operator-(const float* vec) const
  {
    return Vector3<T>(v[0]-vec[0], v[1]-vec[1], v[2]-vec[2]);
  }

  void operator+=(const Vector3<T>& vec) {
    v[0] += vec[0];
    v[1] += vec[1];
    v[2] += vec[2];
  }

  T operator*(const Vector3<T>& vec) const {
    return v[0]*vec[0] + v[1]*vec[1] + v[2]*vec[2];
  }

  bool operator==(const Vector3<T>& vec) {
    return ((v[0] == vec[0]) && (v[1] == vec[1]) && (v[2] == vec[2]));
  }

  bool operator!=(const Vector3<T>& vec) {
    return ((v[0] != vec[0]) || (v[1] != vec[1]) || (v[2] != vec[2]));
  }

  T& operator[](unsigned int i) {
    return v[i];
  }

  const T& operator[](unsigned int i) const {
    return v[i];
  }


  operator const btVector3 () const {
    return btVector3(v[0], v[1], v[2]);
  }


  template<class S>
  friend std::ostream&
  operator<<(std::ostream& os, const Vector3<S>& v);

  template<class S>
  bool operator< (const Vector3<S> &rhs) const {

    if (v[0] != rhs[0])
      return v[0] < rhs[0];

    if (v[1] != rhs[1])
      return v[1] < rhs[1];

    return v[2] < rhs[2];
      
  }

};

template<class S>
Vector3<S> cross(const Vector3<S> &v0, const Vector3<S> &v1) {
  S x,y,z;

  x = v0[1]*v1[2]-v0[2]*v1[1];
  y = v0[2]*v1[0]-v0[0]*v1[2];
  z = v0[0]*v1[1]-v0[1]*v1[0];

  return Vector3<S>(x,y,z);
}

template<class S>
std::ostream&
operator<<(std::ostream& os, const Vector3<S>& v) {
  return os << "( " << v[0] << " " << v[1] << " " << v[2] << " )";
}

template<class S>
void operator >> (const YAML::Node& node, Vector3<S> &v) {
  node[0] >> v[0];
  node[1] >> v[1];
  node[2] >> v[2];
}

template<class S>
Vector3<S> operator*(const S &s, const Vector3<S> &v) {
  return Vector3<S>(v[0]*s, v[1]*s, v[2]*s);
}




typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;

#endif // VECTOR3_H
