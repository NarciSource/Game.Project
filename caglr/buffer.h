#pragma once

#include "glew.h"
#include "glut.h"
#include "renderManager.h"

namespace CAGLR {
	class Buffer
	{
	public:
		Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc);

		/* get */
		const GLuint Num();
		const GLuint Size();

	private:
		/** shader object location */
		GLuint	vertex_attr_loc, normal_attr_loc;

		/** vbo & object_vao_name name */
		GLuint	vertex_vbo_name;
		GLuint	normal_vbo_name;
		GLuint	object_vao_name;

		GLuint	size;

	private:

		void defineVBO(CAGLE::Object* obj);

		void defineVAO();
	};


}