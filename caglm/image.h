#pragma once

#include <string>
#include "array3.h"

namespace CAGLM
{
	class Image
	{
	private:
		int width;
		int height;

		CAGLM::Array3<unsigned char> pixels;	// height*width*3(b,g,r)
	//	CAGLM::Array3<UCHAR> rgbs;		// 3(r,g,b)*height*width


	private:
		Image() {}
		Image(int h, int w);

	public:
		static Image* load_bmp(const std::string filename);

		void close();

		/* get */
		const int Width() { return width; }
		const int Height() { return height; }

		unsigned char*** const Pixels_3dim() { return pixels.p_3dim(); }
		unsigned char** const Pixels_2dim() { return pixels.p_2dim(); }
		unsigned char* const Pixels_1dim() { return pixels.p_1dim(); }
		unsigned char const Pixel(const int x, const int y, const int rgb) { return pixels[x][y][rgb]; }

//		UCHAR* const Rgbs(const int rgb) { return rgbs[0][0]; }
//		UCHAR const Rgb(const int rgb, const int x, const int y) { return rgbs[rgb][x][y]; }

	};
}