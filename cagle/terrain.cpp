#include "terrain.h"

namespace CAGLE {
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

		UCHAR* buffer = new UCHAR[bmp_info.biSizeImage];

		/** read */
		fin.read((char*)buffer, bmp_info.biSizeImage);

		int r, g, b;
		for (int i = 0; i < bmp_info.biHeight; i++)
		{
			for (int j = 0; j < bmp_info.biWidth; j++)
			{
				b = buffer[i * bmp_info.biWidth * 3 + j * 3];
				g = buffer[i * bmp_info.biWidth * 3 + j * 3 + 1];
				r = buffer[i * bmp_info.biWidth * 3 + j * 3 + 2];

				img->pixels[i][j] = Pixel(r, g, b);
			}
		}

		fin.close();
		delete[] buffer;
		return *img;
	}

	void Image::close()
	{
		this->~Image();
	}




	Terrain::Terrain(int h, int w) :length(h), width(w)
	{
		vertexs = new float*[length * 3];
		vertexs[0] = new float[length*width * 3];

		normals = new float*[length * 3];
		normals[0] = new float[length*width * 3];

		indices = new int[(length * 2 - 2)*(width + 1)];

		for (int i = 1; i < length; i++)
		{
			vertexs[i] = vertexs[i - 1] + width * 3;
			normals[i] = normals[i - 1] + width * 3;
		}
	}

	Terrain::~Terrain()
	{
		delete[] vertexs[0];
		delete[] vertexs;
		delete[] normals[0];
		delete[] normals;
		delete[] indices;
	}

	Terrain* Terrain::load_terrain(const std::string filename)
	{
		Image& img = Image::load_bmp(filename);

		/* heights map */
		float** heights_map;

		heights_map = new float*[img.Height()];
		for (int i = 0; i < img.Height(); i++)
		{
			heights_map[i] = new float[img.Width()];
			for (int j = 0; j < img.Width(); j++)
			{
				heights_map[i][j] = img.Pixels(i, j).r / 255.f;
			}
		}

		/* terrain compute */
		Terrain* terrain = new Terrain(img.Height(), img.Width());

		terrain->compute_vertex(heights_map);
		terrain->compute_normal(heights_map);
		terrain->compute_indice();		


		/* clear */
		for (int i = 0; i < img.Height(); i++)
			delete[] heights_map[i];
		delete[] heights_map;
		img.close();

		return terrain;
	}

	void Terrain::compute_vertex(float** hmap)
	{
		static const float scale = 0.1f;

		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < width; j++)
			{
				vertexs[i][3 * j] = i * scale;
				vertexs[i][3 * j + 1] = hmap[i][j];
				vertexs[i][3 * j + 2] = j * scale;
			}
		}
	}

	void Terrain::compute_normal(float** hmap)
	{
		CAGLM::Vec3<float> v1, v2, v3, v4;
		CAGLM::Vec3<float> n1, n2, n3, n4, n;
		for (int i = 0; i < length; i++)
		{
			v1(0, 0, 0); v2(0, 0, 0); v3(0, 0, 0); v4(0, 0, 0);
			n1(0, 0, 0); n2(0, 0, 0); n3(0, 0, 0); n4(0, 0, 0);
			for (int j = 0; j < width; j++)
			{
				if (i != 0) {
					v1 = CAGLM::Vec3<float>(0.f, hmap[i - 1][j] - hmap[i][j], -1.f);
				}
				if (j != 0) {
					v2 = CAGLM::Vec3<float>(-1.f, hmap[i][j - 1] - hmap[i][j], 0.f);
				}
				if (i != length - 1) {
					v3 = CAGLM::Vec3<float>(0.f, hmap[i + 1][j] - hmap[i][j], 1.f);
				}
				if (j != width - 1) {
					v4 = CAGLM::Vec3<float>(1.f, hmap[i][j + 1] - hmap[i][j], 0.f);
				}

				n1 = CAGLM::Vec3<float>::Cross(v1, v2);
				n2 = CAGLM::Vec3<float>::Cross(v2, v3);
				n3 = CAGLM::Vec3<float>::Cross(v3, v4);
				n4 = CAGLM::Vec3<float>::Cross(v4, v1);

				n = CAGLM::Vec3<float>::Normalize(n1 + n2 + n3 + n4);

				normals[i][j * 3] += n.X();
				normals[i][j * 3 + 1] += n.Y();
				normals[i][j * 3 + 2] += n.Z();
			}
		}
	}

	void Terrain::compute_indice()
	{
		int n = 0;
		for (int i = 0; i < length - 1; i++)
		{
			indices[n++] = i*width;
			for (int j = 0; j < width; j++)
			{
				indices[n++] = i*width + j;
				indices[n++] = (i + 1)*width + j;
			}
			indices[n++] = (i + 1)*width + (width - 1);
		}
	}
}