#pragma once

#include <iostream>

#include "glew.h"
#include "glut.h"

#include "resourceManager.h"



namespace CAGLR {
	enum class ShadingType
	{
		Gouraud = 0x01,
		Phong = 0x02,
		Dont = 0x03
	};

	class RenderManager
	{
	public:
		static RenderManager& getInstance(int argc, char* argv[]);
		static RenderManager& getInstance();

	public:
		void display();

		void info();

		void init();

		void setShadingType(ShadingType type);

		int getShadingType() { return static_cast<int>(shadingType); }

		int WindowSizeX() { return windowSizeX; }
		int WindowSizeY() { return windowSizeY; }
		void WindowSizeX(int x) { windowSizeX = x; }
		void WindowSizeY(int y) { windowSizeY = y; }

	private:
		GLint	windowSizeX = 2048;
		GLint	windowSizeY = 1024;

		
		GLuint	ProgramID;

		/** shader object location */
		GLuint	modelmatrix_uni_loc, viewmatrix_uni_loc, porjmatrix_uni_loc;
		GLuint	vertex_attr_loc, normal_attr_loc;
		GLuint	color_uni_loc, light_uni_loc, cam_uni_loc;
		GLuint	shadingtype_loc;

		GLuint	textureID;
		GLuint	texture;

		/** vbo & object_vao_name name */
		GLuint*	vertex_vbo_name;
		GLuint*	normal_vbo_name;
		GLuint*	object_vao_name;
			

		GLuint	sizeObjects;

		CAGLE::ResourceManager& gResMngr = CAGLE::ResourceManager::getInstance();;

		ShadingType	shadingType = ShadingType::Phong;

	private:
		RenderManager(int argc, char* argv[]);


		void defineVBO();

		void defineVAO();

		void render();

		void renderObject(CAGLE::Object* object, int num_object);

		void renderGround(CAGLE::Ground* ground);

		void renderLayout(CAGLE::Object* object);

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	};
}