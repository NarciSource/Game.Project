#pragma once

namespace CAGLM {

	template<typename T>
	class Array2
	{
	private:
		T** arr;
		int dim1, dim2;
		bool copy = false;
	public:
		Array2() {}
		Array2(int dim1, int dim2);
		Array2(T** arr2);
		~Array2();

		void set(int d1, int d2);

		void assign(T in);

		/* get */
		T** const p_2dim()	{ return arr; }
		T* const p_1dim()	{ return arr[0]; }

		T* operator[] (int idx) { return arr[idx]; }

	};

	template<typename T>
	class Array3
	{
	private:
		T*** arr;
		int dim1, dim2, dim3;
	public:
		Array3() {}
		Array3(int dim1, int dim2, int dim3);
		~Array3();

		void set(int d1, int d2, int d3);
		void assign(T in);

		/* get */
		T*** const p_3dim() { return arr; }
		T** const p_2dim() { return arr[0]; }
		T* const p_1dim() { return arr[0][0]; }

		Array2<T>& operator[] (int idx) { return Array2<T>(arr[idx]); }
	};

}

#include "array3.hpp"