#include "buffer.h"


namespace CAGLR {

	Buffer::Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc)
		:vertex_attr_loc(v_loc), normal_attr_loc(n_loc)
	{
		defineVBO(obj);
		defineVAO();
		size = obj->PolygonCount() * 3;
	}

	const GLuint Buffer::Num()
	{
		return object_vao_name;
	}

	const GLuint Buffer::Size()
	{
		return size;
	}

	void Buffer::defineVBO(CAGLE::Object* obj)
	{
		glGenBuffers(1, &vertex_vbo_name);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_name);
		{
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(GLfloat)*obj->PolygonCount() * 9,
				obj->Vertexs(),
				GL_STATIC_DRAW);
		}

		glGenBuffers(1, &normal_vbo_name);
		glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_name);
		{
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(GLfloat)*obj->PolygonCount() * 9,
				obj->Normals(),
				GL_STATIC_DRAW);
		}


		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Buffer::defineVAO()
	{
		glGenVertexArrays(1, &object_vao_name);
		glBindVertexArray(object_vao_name);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_name);
			{
				glVertexAttribPointer(vertex_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}

			glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_name);
			{
				glVertexAttribPointer(normal_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
			glEnableVertexAttribArray(vertex_attr_loc);
			glEnableVertexAttribArray(normal_attr_loc);
		}


		glBindVertexArray(0);
	}
}