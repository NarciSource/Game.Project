#include <algorithm>
#include "terrain.h"

namespace CAGLE {


	void Terrain::init(int l, int w)
	{
		length = l; width = w;
		vertexs.set(l, w, 3);
		normals.set(l, w, 3);

		indices = new int[(l * 2 - 2)*(w + 1)];
	}

	Terrain::~Terrain()
	{
		delete[] indices;
	}

	void Terrain::load_terrain(const std::string filename)
	{
		CAGLM::Image& img = CAGLM::Image::load_bmp(filename);

		/* heights map */
		float** heights_map;

		heights_map = new float*[img.Height()];
		for (int i = 0; i < img.Height(); i++)
		{
			heights_map[i] = new float[img.Width()];
			for (int j = 0; j < img.Width(); j++)
			{
				heights_map[i][j] = static_cast<float>(img.Pixel(i,j,0)) / 255.f;
			}
		}

		/* terrain compute */
		init(img.Height(), img.Width());

		compute_vertex(heights_map);
		compute_normal(heights_map);
		compute_indice();		


		/* clear */
		for (int i = 0; i < img.Height(); i++)
			delete[] heights_map[i];
		delete[] heights_map;
		img.close();
	}

	void Terrain::compute_vertex(float** hmap)
	{
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < width; j++)
			{
				vertexs[i][j][0] = i*1.f;
				vertexs[i][j][1] = hmap[i][j]*curve_level;
				vertexs[i][j][2] = j*1.f;
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

				normals[i][j][0] += n.X();
				normals[i][j][1] += n.Y();
				normals[i][j][2] += n.Z();
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