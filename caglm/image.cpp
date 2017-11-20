#include <fstream>
#include <iostream>
#include <Windows.h>
#include "image.h"

namespace CAGLM
{
	Image::Image(int h, int w) :height(h), width(w)
	{
		pixels.set(h, w, 3);
		rgbs.set(h, w, 3);
	}

	Image& Image::load_bmp(const std::string filename)
	{
		std::ifstream fin(filename, std::ios::in | std::ios::binary);

		/** read header */
		BITMAPFILEHEADER bmp_header;
		BITMAPINFOHEADER bmp_info;

		fin.read((char*)&bmp_header, sizeof(BITMAPFILEHEADER));
		fin.read((char*)&bmp_info, sizeof(BITMAPINFOHEADER));

		/** chk */
		if (bmp_header.bfType != 0x4D42)
		{
			std::cout << "isn't bitmap" << std::endl;
		}
		/*	if (bmp_info.biBitCount != 8) {
		std::cout << "color?" << std::endl;
		}*/


		CAGLM::Image* img = new CAGLM::Image(bmp_info.biHeight, bmp_info.biWidth);

		/** read */
		fin.read((char*)img->pixels[0][0], bmp_info.biSizeImage);

		/** rgb sort */
		UCHAR r, g, b;
		for (int i = 0; i < bmp_info.biHeight; i++)
		{
			for (int j = 0; j < bmp_info.biWidth; j++)
			{
				b = img->pixels[i][j][0];
				g = img->pixels[i][j][1];
				r = img->pixels[i][j][2];

				img->rgbs[0][i][j] = r;
				img->rgbs[1][i][j] = g;
				img->rgbs[2][i][j] = b;
			}
		}

		fin.close();
		return *img;
	}

	void Image::Image::close()
	{
		this->~Image();
	}

}
