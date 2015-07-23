#ifndef VECTOR4_H
#define VECTOR4_H

#include <iostream>
#include <cmath>
#include <yaml-cpp/yaml.h>

#include "Vector3.h"

using namespace std;

template<typename T>
struct Vector4 {

private:

  T v[4];

public:

  Vector4() {v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; v[3] = 0.0; }

  Vector4(const T &x, const T &y, const T &z, const T &w) {
    set(x,y,z,w);
  }

  explicit Vector4(const T *vec) {
    set(vec[0], vec[1], vec[2], vec[3]);
  }

  explicit Vector4(const Vector3<T> &x) {
    v[0] = x[0];
    v[1] = x[1];
    v[2] = x[2];
    v[3] = 1.0;
  }


	Vector4(const LifeSim::VectorDesc &desc) {
		assert(desc.x_size() == 4);
		v[0] = desc.x(0);
		v[1] = desc.x(1);
		v[2] = desc.x(2);
		v[3] = desc.x(3);
	}


  void set(const T &x, const T &y, const T &z, const T &w) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
  }

  T* ptr(void) { return (T*)this; }

  T length() const {
    return sqrt(length_sqr());
  }

  T length_sqr() const {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3];
  }

  T dot(const Vector4<float> &vec) const {
    return v[0]*vec.v[0] + v[1]*vec.v[1] + v[2]*vec.v[2] + v[3]*vec.v[3];
  }

  void scale(const T &s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
    v[3] *= s;
  }

  void normalize()  {
    // Keep an eye on the implicit conversions going on in the next
    // line since sqrt is declared: double sqrt(double x);
    T d = sqrt( length_sqr() );
    v[0] /= d; v[1] /= d; v[2] /= d; v[3] /= d;
  }

  T* get_ptr(void) { return (T*)this; }

  const T* get_ptr(void) const { return (T*)this; }


  T operator*(const Vector4<T> &vec) {
    return vec.dot(*this);
  }

  Vector4<T>
  operator*(double d) const
  {
    return Vector4(v[0]*d, v[1]*d, v[2]*d, v[3]*d);
  }

  Vector4<T>
  operator/(double d) const
  {
    return Vector4(v[0]/d, v[1]/d, v[2]/d, v[3]/d);
  }

  Vector4<T>
  operator+(const Vector4<T>& vec)
  {
    return Vector4<T>(v[0]+vec[0], v[1]+vec[1], v[2]+vec[2], v[3]+vec[3]);
  }

  template <typename Q>
  Vector4<T>
  operator-(const Vector4<Q>& vec) const
  {
    return Vector4<T>(v[0]-vec[0], v[1]-vec[1], v[2]-vec[2], v[3]-vec[3]);
  }


  Vector4<T>
  operator-(const float* vec) const
  {
    return Vector4<T>(v[0]-vec[0], v[1]-vec[1], v[2]-vec[2], v[3]-vec[3]);
  }

  Vector4<T>
  operator-() const {
    return Vector4<T>(-v[0], -v[1], -v[2], -v[3]);
  }

  void operator+=(const Vector4<T>& vec) {
    v[0] += vec[0];
    v[1] += vec[1];
    v[2] += vec[2];
    v[3] += vec[3];
  }


  T operator*(const Vector4<T>& vec) const
  {
    return v[0]*vec[0] + v[1]*vec[1] + v[2]*vec[2] + v[3]*vec[3];
  }

  bool operator==(const Vector4<T>& vec) {
    return ((v[0] == vec[0]) && (v[1] == vec[1]) && (v[2] == vec[2]) && (v[3] == vec[3]));
  }

  bool operator!=(const Vector4<T>& vec) {
    return ((v[0] != vec[0]) || (v[1] != vec[1]) || (v[2] != vec[2]) || (v[3] != vec[3]));
  }

  T& operator[](unsigned int i) {
    return v[i];
  }

  const T& operator[](unsigned int i) const {
    return v[i];
  }

  template<class S>
  friend std::ostream&
  operator<<(std::ostream& os, const Vector4<S>& v);
};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Vector4<S>& v) {
  return os << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
}

template<class S>
void operator >> (const YAML::Node& node, Vector4<S> &v) {
  node[0] >> v[0];
  node[1] >> v[1];
  node[2] >> v[2];
  node[3] >> v[3];
}


typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

#endif // VECTOR4_H
