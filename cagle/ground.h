#pragma once
#include "mvp.h"
#include "terrain.h"
#include "object.h"


namespace CAGLE {
	class Ground : public Object
	{
	private:
		Terrain* terrain;

	public:
		Ground() :Object() { refresh(); }

		void bind(Terrain* trr) { terrain = trr; }

		const float* Normals() { return terrain->Normals(); }
		const float* Vertexs() { return terrain->Vertexs(); }
		const int indices_size() { return terrain->indices_size(); }
		const int* Indices() { return terrain->Indices(); }
		






	};



}