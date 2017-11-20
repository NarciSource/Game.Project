/*************************************************/
/** @CAGLE MVP									**/
/**   ==Chung Ang university GL Engine			**/
/**		ModelViewProjection support				**/
/**												**/
/** @Version : v1.2								**/
/** @Author : Jeong Won Cheol					**/
/** @Contact : jwch11@gmail.com					**/
/**												**/
/** @Create : 14th May 2017						**/
/** @Last Modified: 25th May 2017				**/
/**												**/
/**	@Class : ModelMatrix ViewMatrix				**/
/**			 ProjectionMatrix					**/
/*************************************************/


#pragma once

#include "mat4.h"



namespace CAGLE {
	class ModelMatrix : public CAGLM::Mat4
	{};





	class ViewMatrix : public CAGLM::Mat4
	{
	public:
		/** Pilot View Coordinate System = PVCS 
		* Use eyePoint, roll, yaw and pitch
		* to change direction camera is looking at */
		void pilotView(const CAGLM::Vec3<float> eyePoint, const float roll, const float yaw, const float pitch);





		/** Rectangular Coordinate System = RCS
		* Use eyePoint, focus(at)Point and camera's upVector
		* to change direction camera is looking at 
		* This system also represent slope at focus */
		void lookAt(const CAGLM::Vec3<float> eyePoint, const CAGLM::Vec3<float> atPoint, const CAGLM::Vec3<float> upVector);
	};





	class ProjectionMatrix : public CAGLM::Mat4
	{
	public:
		/** Perspective Projection has depth to be known
		* The arguments define projection space. */
		void frustum(float left, float right, float bottom, float top, float near, float far);
		/*** This is also perspective projection
		* It is easy to understand by fovy and aspect */
		void perspective(float fovy, float aspect, float near, float far);


		/** Parallel Projection has not depth to be known
		* This is all taken no care of depth and is expressed in its original size */
		void parallel(float left, float right, float bottom, float top, float near, float far);


	};






}