#pragma once

#include <iostream>
#include <map>

#include "glew.h"
#include "glut.h"

#include "resourceManager.h"
#include "buffer.h"



namespace CAGLR {
	enum class ShadingType
	{
		Gouraud = 0x01,
		Phong = 0x02,
		Dont = 0x03
	};

	class Buffer;
	class RenderManager
	{
	public:
		void display();

		void info();

		int WindowSizeX() { return windowSizeX; }
		int WindowSizeY() { return windowSizeY; }
		void WindowSizeX(int x) { windowSizeX = x; }
		void WindowSizeY(int y) { windowSizeY = y; }

	private:
		GLint	windowSizeX = 2048;
		GLint	windowSizeY = 1024;
		
		GLuint	ProgramID;
		CAGLE::ResourceManager& gResMngr = CAGLE::ResourceManager::getInstance();


		/** shader object location */
		GLuint	modelmatrix_uni_loc, viewmatrix_uni_loc, porjmatrix_uni_loc;
		GLuint	vertex_attr_loc, normal_attr_loc, uv_attr_loc;
		GLuint	color_uni_loc, light_uni_loc, cam_uni_loc;

		GLuint	texture, texture_uni_loc;

		GLuint	flag_tex_loc, flag_shading_loc;


		/** vao: vertex array object */
		std::map<CAGLE::Object*, Buffer*> buffers;
		

	public:
		static RenderManager& getInstance(int argc, char* argv[]);
		static RenderManager& getInstance();

	private:
		RenderManager(int argc, char* argv[]);

		void init();

		void pushBuffer();

		void render();

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	};
}