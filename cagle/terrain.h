#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>

#include "mat4.h"
#include "array3.h"
#include "image.h"
#include "model.h"

namespace CAGLE {

	class Terrain : public Model
	{
	private:
		int length;
		int width;

		CAGLM::Array3<float>	vertexs;
		CAGLM::Array3<float>	normals;
		CAGLM::Array3<float>	uvs;
		int*		indices;

		const int curve_level = 10;
	private:
		void init(int lh, int w);

		void compute_vertex(CAGLM::Image*);
		void compute_normal(CAGLM::Image*);
		void compute_uv();
		void compute_indice();		

	public:
		Terrain() {}
		~Terrain();

		void loader(const std::string filename);

		

		/* get */
		CAGLM::Vec3<float>	Normal(const int x, const int y) { 
			return CAGLM::Vec3<float>(normals[x][y][0],normals[x][y][1],normals[x][y][2]);
		}
		
		float Height(const int x, const int y) { return vertexs[x][y][1]; }

		const int IndiceSize() { return (length * 2 - 2)*(width + 1); }

		const float*	Vertexs() { return vertexs[0][0]; }
		const float*	Normals() { return normals[0][0]; }
		const float*	Uvs()	  { return uvs[0][0]; }
		const int		PolygonCount() { return IndiceSize() / 3; }
		const int*		Indices() { return indices; }
	};
}