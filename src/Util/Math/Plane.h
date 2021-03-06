#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <cmath>

#include "Vector3.h"

#include "LifeSim.pb.h"


template<typename T>
struct Plane {

public:

	Vector3<T> v;
	T d;

  Plane() { v[0] = v[1] = v[2] = d = 0.0; }

  Plane(const Vector3<T> &_v, const T &_d) {
    set(_v, _d);
  }

	Plane(const LifeSim::VectorDesc &desc) {
		assert(desc.x_size() == 4);

		v[0] = desc.x(0);
		v[1] = desc.x(1);
		v[2] = desc.x(2);
		v.normalize();

		d = desc.x(3);
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


typedef Plane<float> Planef;
typedef Plane<double> Planed;

#endif // PLANE_H
