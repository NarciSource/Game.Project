#include "buffer.h"


namespace CAGLR {

	Buffer::Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc)
		:vertex_attr_loc(v_loc), normal_attr_loc(n_loc)
	{
		defineVBO(obj);
		defineVAO();
		size = obj->PolygonCount() * 3;
	}

	Buffer::Buffer(CAGLE::Object* obj, GLuint v_loc, GLuint n_loc, 
		GLuint uv_loc, GLuint tx, GLuint tx_uni_loc)
		:vertex_attr_loc(v_loc), normal_attr_loc(n_loc),uv_attr_loc(uv_loc),
		texture(tx), texture_uni_loc(tx_uni_loc)
	{
		defineVBO(obj);
		defineVAO();
		size = obj->PolygonCount() * 3;
	}

	const GLuint Buffer::VaoNum()
	{
		return object_vao_name;
	}
	const GLuint Buffer::TextureNum()
	{
		return texture;
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

		if (uv_attr_loc == -1) return;

		glGenBuffers(1, &uv_vbo_name);
		glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_name);
		{
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(GLfloat)*obj->PolygonCount() * 6,
				obj->UV(),
				GL_STATIC_DRAW);
		}


		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				obj->TextureHeight(), obj->TextureWidth(), 0,
				GL_BGR, GL_UNSIGNED_BYTE, obj->Texture());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		glActiveTexture(GL_TEXTURE0);
		

		glBindTexture(GL_TEXTURE_2D, 0);
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

			glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_name);
			{
				glVertexAttribPointer(uv_attr_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}

			glEnableVertexAttribArray(vertex_attr_loc);
			glEnableVertexAttribArray(normal_attr_loc);
			glEnableVertexAttribArray(uv_attr_loc);
		}
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, texture);
		{
		//	glUniform1i(texture_uni_loc, 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		
	}
}