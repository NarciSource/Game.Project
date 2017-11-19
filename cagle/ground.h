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
		

		CAGLM::Vec3<float>	Normal(const int x, const int y) {
			return terrain->Normal(x,y);
		}

		float Height(const float x, const float z) { 
			CAGLM::Vec3<float> p = Position();
			float s = Size();

			int xx = static_cast<int>((x - p.X()) / s * 10);
			int zz = static_cast<int>((z - p.Z()) / s * 10);
			std::cout<<"terr x,z="<< xx << " " << zz << std::endl;


			if (xx < 0 || zz < 0) return 0;
			return terrain->Height(xx, zz)*s+p.Y();
		}




	};



}