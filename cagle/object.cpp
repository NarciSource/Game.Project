/*************************************************/
/** @CAGLE										**/
/**   ==Chung Ang university GL Engine			**/
/*************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include "object.h"

namespace CAGLE {
	Object::Object() :position(CAGLM::Vec3<float>(0, 0, 0)), size(1), yaw(0) {}




	void Object::forward(const float velocity)
	{
		float radian = yaw * M_PI / 180.f;

		CAGLM::Vec3<float> direct(sin(radian), 0, cos(radian));

		position += direct*velocity;
		refresh();
	}


	void Object::around(const float velocity)
	{
		yaw += velocity;
		refresh();
	}





	void Object::refresh()
	{
		modelMatrix.setIdentity();
		modelMatrix.scalef(size);
		modelMatrix.rotatef(yaw, 0, 1, 0);
		modelMatrix.translatev(position);
	}







	void Object::positionRotation(float turnRadius, float velocity, float x, float y, float z)
	{
		int theta = static_cast<int>(velocity / turnRadius * /*glutGet(UT_ELAPSED_TIME) /*/ 100.f) % (360);

		modelMatrix.setIdentity();
		modelMatrix.translatev(-position);
		modelMatrix.translatef(turnRadius, 0, 0);
		modelMatrix.rotatef(1.0f*theta, x, y, z);
		modelMatrix.translatef(-turnRadius, 0, 0);
		modelMatrix.translatev(position);
	}
};