#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <cmath>
#include <fstream>

template<typename T>
struct Tensor {


private:

	// TODO (LOW PRIORITY) : chache aware allocation
	// The tensor data, a single block of memory that stores the tensor
	std::vector<T> v_;

	// The dimensions of the tensor
	vector<uint32_t> dims_;


public:

	// The default constructor is a rank zero tensor
	Tensor() : v_(1, 0) {}

	// Copy of the raw data that makes up a tensor
	Tensor(const std::vector<T> &v, const std::vector<int> &dims) :
		v_(v), dims_(dims){}

	// Copy a vector
	Tensor(const std::vector<T> &v) {
		dims_ = {(uint32_t)v.size()};
		v_ = v;
	}

	// Copy constructor
	Tensor(const Tensor &other) {
		*this = other;
	}

	// Move constructor
	Tensor(Tensor &&other) {
		swap(v_, other.v_);
		swap(dims_, other.dims_);
	}



	// L2 norm
	T norm() const {
		return sqrt(norm_squared());
	}

	// L2 norm squared
	T norm_squared() const {
		return std::accumulate(v_.begin(), v_.end(), 0, [](const T &sum, T &val) { return sum + val*val; });
	}



	// Copy assignment operator
	Tensor& operator=(const Tensor &other) {

		v_    = other.v_;
		dims_ = other.dims_;

		return *this;

	}

	// Move assignment operator
	Tensor& operator=(Tensor &&other) {

		swap(v_,    other.v_);
		swap(dims_, other.dims_);
	
		return *this;

	}





	// Write to file...
	void writeToFile(const string &filename) {

		// Open the file
		std::ofstream f(filename, ios::out | ios::binary);

		// Tensor rank
		uint32_t rank = (uint32_t)dims_.size();

		// Write the rank
		f.write((const char*)&rank, sizeof(uint32_t));

		// Write the dims
		if (rank > 0)
			f.write((const char*)&dims_[0], sizeof(uint32_t) * rank);

		// Write the data
		f.write((const char*)&v_[0], sizeof(T) * v_.size());

	}


};

typedef Tensor<float> Tensorf;
typedef Tensor<double> Tensord;

#endif // TENSOR_H
