#pragma once
#include "mvp.h"
#include "terrain.h"
#include "object.h"
#include "texture.h"


namespace CAGLE {
	class Ground : public Object
	{
	private:
		int colorv;

	public:
		Ground(std::string label) :Object(label),colorv(0x66CC66) { refresh(); }


		const int PolygonCount() { return model->PolygonCount(); }
		const int IndicesSize() { return model->IndiceSize(); }
		const int* Indices() { return model->Indices(); }
		const float* Color();
		


		float Height(const float x, const float z);

	};



}