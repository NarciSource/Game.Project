#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>

#include "mat4.h"
#include "array3.h"
#include "image.h"

namespace CAGLE {

	class Terrain
	{
	private:
		int length;
		int width;

		CAGLM::Array3<float>	vertexs;
		CAGLM::Array3<float>	normals;
		int*		indices;

		const int curve_level = 10;
	private:
		void init(int lh, int w);

		void compute_vertex(float**);
		void compute_normal(float**);
		void compute_indice();		

	public:
		Terrain() {}
		~Terrain();

		void load_terrain(const std::string filename);

		

		/* get */
		CAGLM::Vec3<float>	Normal(const int x, const int y) { 
			return CAGLM::Vec3<float>(normals[x][y][0],normals[x][y][1],normals[x][y][2]);
		}
		
		float Height(const int x, const int y) { return vertexs[x][y][1]; }

		const int indices_size() { return (length * 2 - 2)*(width + 1); }

		float*	Vertexs() { return vertexs[0][0]; }
		float*	Normals() { return normals[0][0]; }
		int*	Indices() { return indices; }
	};
}