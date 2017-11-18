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

#include "caglm.h"
#include "mvp.h"
#include "model.h"

/** Auto GET and Set Function Maker */
#define AUTO_GETSET2(TYPE2, FUNC_NAME2, PROP2) \
		TYPE2 FUNC_NAME2() const \
		{ return PROP2.FUNC_NAME2(); } \
		void FUNC_NAME2(const TYPE2 ARG2) \
		{ PROP2.FUNC_NAME2(ARG2);}










namespace CAGLE {

	/*******************************/
	/* Object                      */
	/*  blabla                     */
	/*******************************/
	class Object {



	public:
		/** Moving operator Funciton*/
		void forward(const float velocity);

		void around(const float velocity);



	private:
		/** Object Name */
		std::string label;


		/** Local Coordinate
		* Polygon Data */
		Model* model;


		/** World Coordinate */
		CAGLM::Vec3<float> position;
		float size;
		float yaw;

		ModelMatrix modelMatrix;

	public:
		Object();



		/** ModelMatrix refersh */
		void refresh();


		void positionRotation(float turnRadius, float velocity, float x, float y, float z);



		/** Set */
		void bind(Model* md) { model = md; }


		/** Get and Set */
		AUTO_GETSET(std::string, hisNameIs, label);
		AUTO_GETSET(std::string, herNameIs, label);
		AUTO_GETSET(CAGLM::Vec3<float>, Position, position);
		AUTO_GETSET2(float, X, position);
		AUTO_GETSET2(float, Y, position);
		AUTO_GETSET2(float, Z, position);
		AUTO_GETSET(float, Size, size);
		AUTO_GETSET(float, Yaw, yaw);


		/** Get */
		const float* loadNormal() { return model->load_normal(); }
		const float* loadVertexPosition() { return model->load_vertex(); }
		const int	 loadPolygonCount() { return model->load_polygon_num(); }
		const float* loadModelMatrix() { return modelMatrix.getElement(); }

		const int	 Color() { return model->load_polygon_color(); }
	};





}