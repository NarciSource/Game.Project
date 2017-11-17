#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>

#include "caglm.h"

namespace CAGLE {
	struct Pixel
	{
		Pixel() {}
		Pixel(int r, int g, int b)
			:r(r), g(g), b(b) {}
		int r, g, b;
	};

	class Image
	{
	private:
		int width;
		int height;

		std::vector<std::vector<Pixel> > pixels;


	private:
		Image(int h, int w);

	public:
		static Image& load_bmp(const std::string filename);

		void close();

		/* get */
		const int Width() { return width; }
		const int Height() { return height; }
		const Pixel Pixels(const int x, const int y) { return pixels[x][y]; }
	};


	class Terrain
	{
	private:
		int length;
		int width;

		float**		vertexs;
		float**		normals;
		int*		indices;

	private:
		void compute_vertex(float**);
		void compute_normal(float**);
		void compute_indice();
		

	public:
		static Terrain* load_terrain(const std::string filename);

		/* get */
		const int Length() { return length; }
		const int Width() { return width; }

		const int indices_size() { return (length * 2 - 2)*(width + 1); }

		float*	Vertexs() { return vertexs[0]; }
		float*	Normals() { return normals[0]; }
		int*	Indices() { return indices; }






		~Terrain();
	private:
		Terrain(int h, int w);
	};
}