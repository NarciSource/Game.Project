/*************************************************/
/** @CAGLE										**/
/**   ==Chung Ang university GL Engine			**/
/**												**/
/** @Version : v1.7								**/
/** @Author : Jeong Won Cheol					**/
/** @Contact : jwch11@gmail.com					**/
/**												**/
/** @Create : 20th May 2017						**/
/** @Last Modified: 9th August 2017				**/
/**												**/
/**	@Class : Object, Camera and Light			**/
/*************************************************/



#pragma once
#include <string>

#include "mat4.h"
#include "mvp.h"
#include "object.h"


/** Auto GET and Set Function Maker */
#define AUTO_GETSET2(TYPE2, FUNC_NAME2, PROP2) \
		TYPE2 FUNC_NAME2() const \
		{ return PROP2.FUNC_NAME2(); } \
		void FUNC_NAME2(const TYPE2 ARG2) \
		{ PROP2.FUNC_NAME2(ARG2);}










namespace CAGLE {

	/*******************************/
	/* Light                       */
	/*  blabla                     */
	/*******************************/
	class Light {

	public:

		void sunset(const float velocity);


	private:
		CAGLM::Vec3<float> position;

		float pitch;


	public:
		Light();



		AUTO_GETSET(CAGLM::Vec3<float>, Position, position);
		AUTO_GETSET(float, Pitch, pitch);
		AUTO_GETSET2(float, X, position);
		AUTO_GETSET2(float, Y, position);
		AUTO_GETSET2(float, Z, position);
	};

}