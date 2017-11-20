#pragma once

#pragma comment(lib,"cagle.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glew32.lib")

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
		GLint	modelMatrixID, viewMatrixID, projectionMatrixID;
		GLint	vertexPositionID, normalID;
		GLint	colorID, lightID, cameraID;
		GLint	shadingTypeID;
		GLint	textureID;
		GLuint	texture;

		ShadingType	shadingType = ShadingType::Phong;

	private:
		RenderManager(int argc, char* argv[]);

		void init();

		void render();

		void renderObject(CAGLE::Object* object);

		void renderGround(CAGLE::Ground* ground);

		void renderLayout(CAGLE::Object* object);

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	};
}