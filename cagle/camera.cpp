/*************************************************/
/** @CAGLE										**/
/**   ==Chung Ang university GL Engine			**/
/*************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include "camera.h"

namespace CAGLE {

	Camera::Camera() : size(1), cameraUp(CAGLM::Vec3<float>(0, 1, 0)),
		roll(0), yaw(0), pitch(0), fovy(1),
		aspect(2.0), neear(0.1f), faar(100.f),
		projectionType(PROJECTION_PERSPECTIVE), flushMap(false)
	{
		shutter();

		projectionMatrix.frustum(left, right, bottom, top, neear, faar);
	}





	void Camera::lookAround(const int direct, const float velocity)
	{
		CAGLM::Mat4 rotateMat;
		
		switch (direct)
		{
		case CAGLE_LEFT:
			rotateMat.rotatef(velocity, 0, -1, 0);
			break;
		case CAGLE_RIGHT:
			rotateMat.rotatef(velocity, 0, 1, 0);
			break;
		case CAGLE_UP:
			lookAt.Y(lookAt.Y() + velocity);
			break;
		case CAGLE_DOWN:
			lookAt.Y(lookAt.Y() - velocity);
			break;
		}

		CAGLM::Vec3<float> v = lookAt - position;
		lookAt = rotateMat*v + position;

		
		shutter();
	}




	void Camera::fovyUp(const float value)
	{
		fovy += value * M_PI / 180;
		if (fovy > 2.0) fovy = 2.0;
		shutter();
	}



	void Camera::fovyDown(const float value)
	{
		fovy -= value * M_PI / 180;
		if (fovy < 0.0) fovy = 0.0;
		shutter();
	}




	void Camera::projectionSwitch(const int type)
	{

		if (projectionType == PROJECTION_MAPVIEW && type == PROJECTION_MAPVIEW)
			projectionType = PROJECTION_PERSPECTIVE;
		else projectionType = type;
		projectionRefresh();
	}


	void Camera::projectionRefresh(void)
	{

		switch (projectionType)
		{

		case PROJECTION_MAPVIEW:
			viewMatrix.lookAt(CAGLM::Vec3<float>(0, 50, 0), CAGLM::Vec3<float>(0, 0, 0), CAGLM::Vec3<float>(0, 0, 1));
			viewMatrix.scalef(0.005f);
			projectionMatrix.parallel(left, right, bottom, top, neear, faar);
			break;


		case PROJECTION_PERSPECTIVE:
			viewMatrix.lookAt(position, lookAt, cameraUp);
			projectionMatrix.frustum(left, right, bottom, top, neear, faar);
			break;


		case PROJECTION_ORTHOGONAL:
			viewMatrix.lookAt(position, lookAt, cameraUp);
			projectionMatrix.parallel(left, right, bottom, top, neear, faar);
			break;

		}
	}


	void Camera::shutter()
	{
		/** Adjust the view box */
		top = -2 * neear*tan(fovy / 2);
		bottom = -top;
		right = aspect * top;
		left = -right;


		viewMatrix.scalef(size);

		/** Proj and View matrix refresh */
		projectionRefresh();
	}
};