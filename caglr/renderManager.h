#pragma once

#pragma comment(lib,"cagle.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glew32.lib")

#include <iostream>

#include "glew.h"
#include "glut.h"

#include "resourceManager.h"



namespace CAGLR {
	class RenderManager
	{
	public:
		static RenderManager& getInstance(int argc, char* argv[]);
		static RenderManager& getInstance();

	public:
		void display();

		void setShadingType(int type);

	private:
		GLint	windowSizeX = 2048;
		GLint	windowSizeY = 1024;

		GLuint	ProgramID;
		GLint	modelMatrixID, viewMatrixID, projectionMatrixID;
		GLint	vertexPositionID, normalID;
		GLint	colorID, lightID, cameraID;
		GLint	shadingTypeID;
		GLuint	texture;

		int		shadingType = 0x02;

	private:
		RenderManager(int argc, char* argv[]);

		void init();

		void render();

		void renderObject(CAGLE::Object* object);

		void renderTerrain(CAGLE::Terrain* terrain);

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	};
}