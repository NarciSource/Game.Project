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
		T e[3];

	public:
		Vec3() {}
		Vec3(const T x, const T y, const T z)
		{
			e[0] = x; e[1] = y; e[2] = z;
		}



		const T length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }

		/** Arithmetic operator */
		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		Vec3 operator+(const Vec3& v) const { return Vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]); }
		Vec3& operator+=(const Vec3& v) { e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2]; return *this; }
		Vec3 operator-(const Vec3& v) const { return Vec3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]); }
		Vec3& operator-=(const Vec3& v) { e[0] -= v.e[0]; e[1] -= v.e[1]; e[2] -= v.e[2]; return *this; }
		Vec3 operator*(const T num) const { return Vec3(e[0]*num, e[1]*num, e[2]*num); }
		Vec3& operator*=(const Vec3& v) { e[0] *= v.e[0]; e[1] *= v.e[1]; e[2] *= v.e[2]; return *this; }
		Vec3 operator/(const T num) const
		{
			if (num == 0) throw - 1;
			else return Vec3(e[0] / num, e[1] / num, e[2] / num);
		}
		Vec3& operator/=(const Vec3& v) { e[0] /= v.e[0]; e[1] /= v.e[1]; e[2] /= v.e[2]; return *this; }


		/** Relational operator */
		bool operator==(const Vec3& v) const { return (e[0] == v.e[0])&(e[1] == v.e[1])&(e[2] == v.e[2]); }
		bool operator!=(const Vec3& v) const { return !((e[0] == v.e[0])&(e[1] == v.e[1])&(e[2] == v.e[2])); }


		/** Vector operator */
		static Vec3 Normalize(const Vec3 v) { return Vec3(v.e[0] / v.length(), v.e[1] / v.length(), v.e[2] / v.length()); }
		static T Dot(const Vec3& v1, const Vec3& v2) { return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2]; }
		static Vec3 Cross(const Vec3& v1, const Vec3& v2) { return Vec3(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1], v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2], v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]); }
		static T Angle(const Vec3& v1, const Vec3& v2) { return acosf(Dot(v1, v2) / (v1.length()*v2.length())); }

		/** Set */
		void operator()(const T argx, const T argy, const T argz) { e[0] = argx; e[1] = argy; e[2] = argz; }

		/** GET and SET */
		AUTO_GETSET(T, X, e[0]);
		AUTO_GETSET(T, Y, e[1]);
		AUTO_GETSET(T, Z, e[2]);

		const float* get_area() { return e; }
	};
}