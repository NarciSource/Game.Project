#pragma once
#include "object.h"
#include "array3.h"
#include "texture.h"

namespace CAGLE {
	class Sky :public Object
	{
	public:
		Sky(std::string name) :Object(name) {}
	};
}