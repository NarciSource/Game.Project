#pragma once

#include "object.h"
#include "camera.h"

#include "ground.h"

namespace CAGLE {
	enum class Direct {
		Forward, Backward,
		Left, Right
	};


	class Player {
	private:
		Camera* eye;

		

	public:
		Player() {}

		void bind(Camera* pixed_camera) { eye = pixed_camera; }

		void move(Ground* ground, const Direct direct, const float velocity)
		{
			CAGLM::Vec3<float> v = eye->LookAt() - eye->Position();
			CAGLM::Vec3<float> n;
			
			float height = ground->Height(eye->Position().X(), eye->Position().Z());
			v.Y(0);
			std::cout << "height = " << height<<std::endl;
			v = CAGLM::Vec3<float>::Normalize(v);
			n = CAGLM::Vec3<float>::Cross(v, CAGLM::Vec3<float>(0, 1, 0));

			switch (direct)
			{
			case Direct::Forward:
				eye->Position(eye->Position() + v*velocity);
				eye->Position(CAGLM::Vec3<float>(eye->Position().X(),height,eye->Position().Z()));
				eye->LookAt(eye->LookAt() + v*velocity);
				break;
			case Direct::Backward:
				eye->Position(eye->Position() - v*velocity);
				eye->LookAt(eye->LookAt() - v*velocity);
				break;
			case Direct::Left:
				eye->Position(eye->Position() + n*velocity);
				eye->LookAt(eye->LookAt() + n*velocity);
				break;
				break;
			case Direct::Right:
				eye->Position(eye->Position() - n*velocity);
				eye->LookAt(eye->LookAt() - n*velocity);
				break;
				break;
			default:
				throw;
			}
			eye->shutter();
			
		}

		void lookAround(const int direct, const float velocity)
		{
			eye->lookAround(direct, velocity);
		}
	};


}