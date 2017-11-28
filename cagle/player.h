#pragma once

#include "object.h"
#include "camera.h"

namespace CAGLE {
	enum class Direct {
		Forward, Backward,
		Left, Right
	};


	class Player {
	private:
		Camera* eye;

		const float player_tall = 30.f;
	public:
		Player() {}

		void bind(Camera* pixed_camera) { eye = pixed_camera; }

		void move(const Direct direct, const float velocity)
		{
			CAGLM::Vec3<float> v = eye->LookAt() - eye->Position();
			CAGLM::Vec3<float> n;

			
			v.Y(0);

			v = CAGLM::Vec3<float>::Normalize(v);
			n = CAGLM::Vec3<float>::Cross(v, CAGLM::Vec3<float>(0, 1, 0));

			switch (direct)
			{
			case Direct::Forward:
				eye->Position(eye->Position() + v*velocity);
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
			case Direct::Right:
				eye->Position(eye->Position() - n*velocity);
				eye->LookAt(eye->LookAt() - n*velocity);
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

		float Tall()
		{
			return player_tall;
		}

		CAGLM::Vec3<float> Position()
		{
			return eye->Position();
		}
	private:
		void Position(CAGLM::Vec3<float> position)
		{
			eye->Position(position);
			eye->shutter();
		}
		friend class ResourceManager;
	};


}