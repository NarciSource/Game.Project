#include <fstream>
#include <iostream>

#include "texture.h"

namespace CAGLE {
	void Texture::texture_loader(std::string filename)
	{
		std::ifstream fin(filename);
		if (!fin.is_open()) {
			std::cerr << "no file: " << filename << std::endl;
			return;
		}
		else {
			std::clog << "file open: " << filename << std::endl;
		}

		texture_img = CAGLM::Image::load_bmp(filename);
	}

	float Texture::Height() { return texture_img->Height()*1.f; }
	float Texture::Width() { return texture_img->Width()*1.f; }
	unsigned char* Texture::Pixels() { return texture_img->Pixels_1dim(); }

}