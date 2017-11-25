#pragma once
#include "mvp.h"
#include "terrain.h"
#include "object.h"


namespace CAGLE {
	class Ground : public Object
	{
	private:
		Terrain* terrain;
		int colorv;

	public:
		Ground(std::string label) :Object(label),colorv(0x66CC66) { refresh(); }

		void bind(Terrain* trr) { terrain = trr; }

		const float* Normals() { return terrain->Normals(); }
		const float* Vertexs() { return terrain->Vertexs(); }
		const float* UV() { return nullptr; }
		const int PolygonCount() { return terrain->indices_size()/3; }
		const int indices_size() { return terrain->indices_size(); }
		const int* Indices() { return terrain->Indices(); }
		const float* Color();
		

		CAGLM::Vec3<float>	Normal(const int x, const int y) { return terrain->Normal(x,y); }


		float Height(const float x, const float z);

	};



}