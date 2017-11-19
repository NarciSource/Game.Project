#include "player.h"

namespace CAGLE {

	void Player::move(const Direct direct, const float velocity)
	{
		moveBehavior.move(eye, direct, velocity);
	}

	void Player::bind(Camera* pixedCamera)
	{
		eye = pixedCamera;
	}

	void MoveBehavior::move(Camera* ref, const Direct direct, const float velocity)
	{
		CAGLM::Vec3<float> v = ref->LookAt() - ref->Position();
		/*
		switch (direct)
		{
		case Direct::Forward:
			position += v * velocity;
			lookAt += v * velocity;
			break;
		case Direct::Backward:
			position -= v * velocity;
			lookAt -= v * velocity;
			break;
		case Direct::Left:
			position += n * velocity;
			lookAt += n * velocity;
			break;
		case Direct::Right:
			position -= n * velocity;
			lookAt -= n * velocity;
			break;
		case Direct::Up:
			position.Y(position.Y() + 5 * velocity);
			lookAt.Y(lookAt.Y() + 5 * velocity);
			break;
		case Direct::Down:
			position.Y(position.Y() - 5 * velocity);
			lookAt.Y(lookAt.Y() - 5 * velocity);
			break;
		default:
			throw
		}*/
	}
}