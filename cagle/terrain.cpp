#include "terrain.h"

Image::Image(int h, int w) :height(h), width(w)
{
	for (int i = 0; i < height; i++)
	{
		std::vector<Pixel> each;
		each.resize(width);
		pixels.push_back(each);
	}
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
	if (bmp_info.biBitCount != 8) {
		std::cout << "color?" << std::endl;
	}


	Image* img = new Image(bmp_info.biHeight, bmp_info.biWidth);


//	RGBQUAD hRGB[256];

	UCHAR* data = new UCHAR[bmp_info.biSizeImage];
	fin.read((char*)data, bmp_info.biSizeImage);

	int r, g, b;
	for (int i = 0; i < bmp_info.biHeight; i++)
	{
		for (int j = 0; j < bmp_info.biWidth; j++)
		{
			b = data[i * bmp_info.biWidth + j*3];
			g = data[i * bmp_info.biWidth + j*3 + 1];
			r = data[i * bmp_info.biWidth + j*3 + 2];

			img->pixels[i][j] = Pixel(r, g, b);
		}
	}

	fin.close();
	delete[] data;
	return *img;
}

void Image::close()
{
	this->~Image();
}




Terrain::Terrain(int h, int w) :width(w), length(h)
{
	for (int i = 0; i < length; i++)
	{
		std::vector<float> each;
		each.resize(width);
		heights_map.push_back(each);
	}
}

Terrain* Terrain::load_terrain(const std::string filename)
{
	Image& img = Image::load_bmp(filename);

	Terrain* terrain = new Terrain(img.Height(), img.Width());

	for (int i = 0; i < img.Height(); i++)
	{
		for (int j = 0; j < img.Width(); j++)
		{
			terrain->heights_map[i][j] = img.Pixels(i, j).r / 255.f;
		}
	}

	img.close();
	return terrain;
}