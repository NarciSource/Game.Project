#pragma once

namespace CAGLM
{
	template<typename T>
	Array2<T>::Array2(int dim1, int dim2)
		:dim1(dim1), dim2(dim2)
	{
		set(dim1, dim2);
	}

	template<typename T>
	Array2<T>::Array2(T** arr2)
	{
		arr = arr2;
		copy = true;
	}

	template<typename T>
	Array2<T>::~Array2()
	{
		if (copy) return;
		delete[] arr[0];
		delete[] arr;
	}

	template<typename T>
	void Array2<T>::set(int d1, int d2)
	{
		if (arr == nullptr)
		{
			dim1 = d1; dim2 = d2; dim3 = d3;
			arr = new T*[dim1];
			arr[0] = new T[dim1*dim2];

			for (int i = 1; i < dim1; i++)
				arr[i] = arr[i - 1] + dim2;
		}
	}

	template<typename T>
	void Array2<T>::assign(T in)
	{
		for (int i = 0; i < dim1*dim2; i++)
			arr[0][i] = in;
	}


	template<typename T>
	Array3<T>::Array3(int dim1, int dim2, int dim3)
		:dim1(dim1), dim2(dim2), dim3(dim3)
	{
		set(dim1, dim2, dim3);
	}

	template<typename T>
	Array3<T>::~Array3()
	{
		delete[] arr[0][0];
		delete[] arr[0];
		delete[] arr;
	}

	template<typename T>
	void Array3<T>::set(int d1, int d2, int d3)
	{
		if (arr == nullptr)
		{
			dim1 = d1; dim2 = d2; dim3 = d3;

			arr = new T**[dim1];
			arr[0] = new T*[dim1*dim2];
			arr[0][0] = new T[dim1*dim2*dim3];

			for (int i = 1; i < dim1; i++)
				arr[i] = arr[i - 1] + dim2;
			for (int i = 1; i < dim1*dim2; i++)
				arr[0][i] = arr[0][i - 1] + dim3;
		}
	}

	template<typename T>
	void Array3<T>::assign(T in)
	{
		for (int i = 0; i < dim1*dim2*dim3; i++)
			arr[0][0][i] = in;
	}
}