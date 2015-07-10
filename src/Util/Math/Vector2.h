#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include <cmath>

template<typename T>
struct Vector2 {

private:
  T v[2];

public:

  Vector2() {v[0] = 0.0; v[1] = 0.0; }

  Vector2(const T &x, const T &y) {
    set(x,y);
  }

  Vector2(const T *vec) {
    set(vec[0], vec[1]);
  }


  void set(const T &x, const T &y) {
    v[0] = x;
    v[1] = y;
  }

  T* ptr(void) { return (T*)this; }

  T length() const {
    return sqrt(length_sqr());
  }

  T length_sqr() const {
    return v[0]*v[0] + v[1]*v[1];
  }

  T dot(const Vector2<float> &vec) const {
    return v[0]*vec.v[0] + v[1]*vec.v[1];
  }

  void scale(const T &s) {
    v[0] *= s;
    v[1] *= s;
  }

  void normalize() {
    T d = sqrt( length_sqr() );
    v[0] /= d; v[1] /= d;
  }

  T* get_ptr(void) { return (T*)this; }

  Vector2<T>
  operator*(double d) const
  {
    return Vector2(v[0]*d, v[1]*d);
  }

  Vector2<T>
  operator/(double d) const
  {
    return Vector2(v[0]/d, v[1]/d);
  }

  Vector2<T>
  operator+(const Vector2<T>& vec)
  {
    return Vector2<T>(v[0]+vec[0], v[1]+vec[1]);
  }

  template <typename Q>
  Vector2<T>
  operator-(const Vector2<Q>& vec) const
  {
    return Vector2<T>(v[0]-vec[0], v[1]-vec[1]);
  }


  Vector2<T>
  operator-(const float* vec) const
  {
    return Vector2<T>(v[0]-vec[0], v[1]-vec[1]);
  }

  void operator+=(const Vector2<T>& vec) {
    v[0] += vec[0];
    v[1] += vec[1];
  }


  T operator*(const Vector2<T>& vec) const
  {
    return v[0]*vec[0] + v[1]*vec[1];
  }

  bool operator==(const Vector2<T>& vec) {
    return ((v[0] == vec[0]) && (v[1] == vec[1]));
  }

  bool operator!=(const Vector2<T>& vec) {
    return ((v[0] != vec[0]) || (v[1] != vec[1]));
  }

  T& operator[](unsigned int i) {
    return v[i];
  }

  const T& operator[](unsigned int i) const {
    return v[i];
  }

  template<class S>
  friend std::ostream&
  operator<<(std::ostream& os, const Vector2<S>& v);
};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Vector2<S>& v) {
  return os << "( " << v[0] << " " << v[1] << " )";
}

template<class S>
void operator >> (const YAML::Node& node, Vector2<S> &v) {
  node[0] >> v[0];
  node[1] >> v[1];
}

template<class S>
Vector2<S> operator*(const S &s, const Vector2<S> &v) {
  return Vector2<S>(v[0]*s, v[1]*s);
}


typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

#endif // VECTOR2_H
