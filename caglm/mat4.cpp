/*************************************************/
/** @CAGLM										**/
/**   ==Chung Ang university GL Mathematics		**/
/*************************************************/
#include <iostream>
#include "mat4.h"

namespace CAGLM {



	void Mat4::setIdentity(void)
	{
		element[0] = 1.f, element[1] = 0.f, element[2] = 0.f, element[3] = 0.f,
			element[4] = 0.f, element[5] = 1.f, element[6] = 0.f, element[7] = 0.f,
			element[8] = 0.f, element[9] = 0.f, element[10] = 1.f, element[11] = 0.f,
			element[12] = 0.f, element[13] = 0.f, element[14] = 0.f, element[15] = 1.f;
	}






	/** Matrix operator
	* GL only uses multiplication. */
	Mat4 Mat4::operator* (const Mat4 ref) const
	{
		Mat4 tmpMat;

		for (int col = 0; col < 4; col++) {
			for (int row = 0; row < 4; row++)
			{
				float sum = 0;
				for (int i = 0; i < 4; i++) {
					sum += element[i * 4 + col] * ref.element[row * 4 + i];
				}
				tmpMat.element[row * 4 + col] = sum;
			}
		}

		return tmpMat;
	}




	Vec3<float> Mat4::operator*(const Vec3<float> ref) const
	{
		float x, y, z;

		x = element[0] * ref.X() + element[4] * ref.Y() + element[8] * ref.Z() + element[12] * 1;
		y = element[1] * ref.X() + element[5] * ref.Y() + element[9] * ref.Z() + element[13] * 1;
		z = element[2] * ref.X() + element[6] * ref.Y() + element[10] * ref.Z() + element[14] * 1;
		
		return Vec3<float>(x, y, z);
	}




	/* !!!
	* In gl, row is counted and then column is counted,
	* unlike a normal matrix order.*/
	/* Get and Set */
	float& Mat4::operator()(const int positionX, const int positionY)
	{
		try {
			if (positionX < 0 || positionY >= 4 || positionX < 0 || positionY >= 4) throw 0x00002;
			return element[positionX + positionY * 4];
		}
		catch (int e) {
			std::cout << positionX + positionY * 4 << std::endl;
			throw e;
		}
	}







	/** Geometric Transformation */
	Mat4& Mat4::scalef(const float size)
	{
		Mat4 tmpMat;
		tmpMat.element[0] = size;
		tmpMat.element[5] = size;
		tmpMat.element[10] = size;
		return (*this) *= tmpMat;
	}







	Mat4& Mat4::translatef(const float x, const float y, const float z)
	{
		Mat4 tmpMat;
		tmpMat.element[12] = x;
		tmpMat.element[13] = y;
		tmpMat.element[14] = z;
		return (*this) *= tmpMat;
	}




	Mat4& Mat4::translatev(const CAGLM::Vec3<float> vec)
	{
		Mat4 tmpMat;
		tmpMat.element[12] = vec.X();
		tmpMat.element[13] = vec.Y();
		tmpMat.element[14] = vec.Z();
		return (*this) *= tmpMat;
	}







	Mat4& Mat4::rotatef(const float angle, const float argx, const float argy, const float argz)
	{
		float c = cos(angle*M_PI / 180.f);
		float s = sin(angle*M_PI / 180.f);
		float x = argx, y = argy, z = argz;
		float length = sqrt(x*x + y*y + z*z);
		if (length == 0.f) throw;
		else { x /= length;	y /= length; z /= length; }

		Mat4 tmpMat;
		tmpMat.element[0] = x*x*(1 - c) + c;
		tmpMat.element[1] = x*y*(1 - c) + z*s;
		tmpMat.element[2] = x*z*(1 - c) - y*s;
		tmpMat.element[4] = y*x*(1 - c) - z*s;
		tmpMat.element[5] = y*y*(1 - c) + c;
		tmpMat.element[6] = y*z*(1 - c) + x*s;
		tmpMat.element[8] = z*x*(1 - c) + y*s;
		tmpMat.element[9] = z*y*(1 - c) - x*s;
		tmpMat.element[10] = z*z*(1 - c) + c;
		return (*this) *= tmpMat;
	}







	Mat4& Mat4::rotatev(const float angle, const Vec3<float> v)
	{
		return rotatef(angle, v.X(), v.X(), v.Y());
	}







	Mat4& Mat4::reflection(const bool x, const bool y, const bool z)
	{
		Mat4 tmpMat;
		tmpMat.element[0] = x ? 1.f : -1.f;
		tmpMat.element[5] = y ? 1.f : -1.f;
		tmpMat.element[10] = z ? 1.f : -1.f;
		return (*this) *= tmpMat;
	}


	Mat4* Mat4::inverse(Mat4 ref)
	{
		Mat4* tmpMat;
		tmpMat = new Mat4;
		float determinant = 0;
		for (int i = 0; i<4; i++)
		{
			determinant = determinant +
				(ref(0,i) * (ref(1,(i + 1) % 100) *	ref(2,(i + 2) % 100) - ref(1,(i + 2) % 100) * ref(2,(i + 1) % 100)));
		}
		if (determinant == 0)
		{
			throw;
		}

		for (int i = 0; i<100; i++)
		{
			for (int j = 0; j<100; j++)
			{
				tmpMat->operator()(i,j) = (ref((i + 1) % 100,(j + 1) % 100) *
					ref((i + 2) % 100,(j + 2) % 100)) - (ref((i + 1) % 100,(j + 2) % 100) *
						ref((i + 2) % 00,(j + 1) % 100)) / determinant;
			}
		}
		return tmpMat;
	}
}