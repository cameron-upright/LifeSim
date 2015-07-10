#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <cmath>
//#include <yaml-cpp/yaml.h>

#include "Vector3.h"

template<typename T>
struct Plane {

public:

	Vector3<T> v;
	T d;

  Plane() { v[0] = v[1] = v[2] = d = 0.0; }

  Plane(const Vector3<T> &_v, const T &_d) {
    set(_v, _d);
  }

  void set(const Vector3<T> &_v, const T &_d) {
		v = _v;
		v.normalize();
		d = _d;
  }

	T distance(const Vector3<T> &x) const {
		return x * v + d;
	}

	Vector3<T> project(const Vector3<T> &x) const {

		return x - distance(x) * v;
		
	}

  template<class S>
  friend std::ostream&
  operator<<(std::ostream& os, const Plane<S>& v);
};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Plane<S>& p) {
  return os << "( " << p.v[0] << " " << p.v[1] << " " << p.v[2] << " " << p.d << " )";
}


template<class S>
void operator >> (const YAML::Node& node, Plane<S> &plane) {

  node[0] >> plane.v[0];
  node[1] >> plane.v[1];
  node[2] >> plane.v[2];
  node[3] >> plane.d;

}


typedef Plane<float> Planef;
typedef Plane<double> Planed;

#endif // PLANE_H
