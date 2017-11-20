#pragma once

#include "vec3.h"

/** Auto GET and Set Function Maker */
#define AUTO_GETSET(TYPE, FUNC_NAME, PROP) \
		TYPE FUNC_NAME() const \
		{ return PROP; } \
		void FUNC_NAME(const TYPE ARG) \
		{ PROP = ARG; }
#define M_PI static_cast<float>(3.14159265358979323846)


namespace CAGLM {
	/*******************************************
	******* 4dim Matrix ************************
	* Matrix projected 3 dim space to 4 dim,
	* in order to support to the Homogeneous coordinate.
	*******************************************/
	class Mat4 {
	private:
		float element[4 * 4];

	public:
		Mat4() { setIdentity(); }

		void setIdentity(void);



		/** Matrix operator
		* GL only uses multiplication. */
		Mat4 operator* (const Mat4 ref) const;

		/* !!!
		* This operator differs from normal that.
		* It muliplied from back to front, to aid in gl carcuration.*/
		Mat4& operator*=(const Mat4 ref) { return *this = ref * (*this); }


		Vec3<float> operator*(const Vec3<float> ref) const;



		/* !!!
		* In gl, row is counted and then column is counted,
		* unlike a normal matrix order.*/
		/* Get and Set */
		float& operator()(const int positionX, const int positionY);





		/** Get */
		const float* getElement(void) const { return element; }




		/** Geometric Transformation */
		Mat4& scalef(const float size);

		Mat4& translatef(const float x, const float y, const float z);
		Mat4& translatev(const Vec3<float> vec);

		Mat4& rotatef(const float angle, const float x, const float y, const float z);
		Mat4& rotatev(const float angle, const Vec3<float> v);

		Mat4& reflection(const bool x, const bool y, const bool z);



		static Mat4* inverse(Mat4 ref);
	};

}