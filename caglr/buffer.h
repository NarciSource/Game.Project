#pragma once

#include "glew.h"
#include "glut.h"
#include "renderManager.h"

namespace CAGLR {
	class Buffer
	{
	public:
		Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc);
		Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc, 
			GLuint uv_loc, GLuint tx, GLuint tx_uni_loc);

		/* get */
		const GLuint VaoNum();
		const GLuint TextureNum();
		const GLuint Size();

	private:
		/** shader object location */
		GLuint	vertex_attr_loc;
		GLuint	normal_attr_loc;
		GLuint	uv_attr_loc=-1;
		GLuint	texture_uni_loc;

		GLuint	texture;

		/** vbo & object_vao_name name */
		GLuint	vertex_vbo_name;
		GLuint	normal_vbo_name;
		GLuint	uv_vbo_name;

		GLuint	object_vao_name;

		GLuint	size;

	private:

		void defineVBO(CAGLE::Object* obj);

		void defineVAO();
	};


}