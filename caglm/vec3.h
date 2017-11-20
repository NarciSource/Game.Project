#pragma once

#include <cmath>

/** Auto GET and Set Function Maker */
#define AUTO_GETSET(TYPE, FUNC_NAME, PROP) \
		TYPE FUNC_NAME() const \
		{ return PROP; } \
		void FUNC_NAME(const TYPE ARG) \
		{ PROP = ARG; }
#define M_PI static_cast<float>(3.14159265358979323846)

namespace CAGLM {





	/*******************************************
	******* 3dim Vector ************************
	* This vector has three coordinate values
	* It can be read and write from outside.
	*******************************************/

	template<typename T> //Vec3 is used as direction vector(float) with a triple container(int).
	class Vec3 {
	public:
		T x, y, z;

	public:
		Vec3() {}
		Vec3(const T x, const T y, const T z) : x(x), y(y), z(z) {}



		const T length() const { return sqrt(x*x + y*y + z*z); }

		/** Arithmetic operator */
		Vec3 operator-() const { return Vec3(-x, -y, -z); }
		Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
		Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
		Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		Vec3 operator*(const T num) const { return Vec3(x*num, y*num, z*num); }
		Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		Vec3 operator/(const T num) const
		{
			if (num == 0) throw - 1;
			else return Vec3(x / num, y / num, z / num);
		}
		Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }


		/** Relational operator */
		bool operator==(const Vec3& v) const { return (x == v.x)&(y == v.y)&(z == v.z); }
		bool operator!=(const Vec3& v) const { return !((x == v.x)&(y == v.y)&(z == v.z)); }


		/** Vector operator */
		static Vec3 Normalize(const Vec3 v) { return Vec3(v.x / v.length(), v.y / v.length(), v.z / v.length()); }
		static T Dot(const Vec3& v1, const Vec3& v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
		static Vec3 Cross(const Vec3& v1, const Vec3& v2) { return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x); }
		static T Angle(const Vec3& v1, const Vec3& v2) { return acosf(Dot(v1, v2) / (v1.length()*v2.length())); }

		/** Set */
		void operator()(const T argx, const T argy, const T argz) { x = argx; y = argy; z = argz; }

		/** GET and SET */
		AUTO_GETSET(T, X, x);
		AUTO_GETSET(T, Y, y);
		AUTO_GETSET(T, Z, z);
	};
}