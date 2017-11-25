#pragma once

#include "image.h"

namespace CAGLE {
	class Texture {
	private:
		CAGLM::Image* texture_img;
		int	color;

	public:
		void texture_loader(std::string filename);

		float Height();
		float Width();
		unsigned char* Pixels();
	};


}