#include "renderManager.h"

namespace CAGLR {
	RenderManager& RenderManager::getInstance(int argc, char* argv[])
	{
		static RenderManager instance(argc, argv);
		return instance;
	}

	RenderManager& RenderManager::getInstance()
	{
		static char *argv[1] = { (char*)"anything" };
		return getInstance(1, argv);
	}

	void RenderManager::display()
	{
		glutDisplayFunc([] {
			RenderManager::getInstance().render();
		});
	}

	void RenderManager::setShadingType(int type)
	{
		shadingType = type;
	}




	RenderManager::RenderManager(int argc, char* argv[])
	{
		/** glut, windows setting */
		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(windowSizeX, windowSizeY);
		glutCreateWindow("GAME");

		init();
	}


	void RenderManager::init()
	{
		/** gl setting */
		glEnable(GL_DEPTH_TEST);
		int glewtest = glewInit();
		if (glewtest != GLEW_OK) {
			std::cerr << "glew error" << std::endl;
		}

		ProgramID = LoadShaders("shader\\VertexShader.vert", "shader\\FragmentShader.frag");

		modelMatrixID = glGetUniformLocation(ProgramID, "modelMatrix");
		viewMatrixID = glGetUniformLocation(ProgramID, "viewMatrix");
		projectionMatrixID = glGetUniformLocation(ProgramID, "projectionMatrix");

		vertexPositionID = glGetAttribLocation(ProgramID, "vertexPosition");
		normalID = glGetAttribLocation(ProgramID, "vertexNormal");

		colorID = glGetUniformLocation(ProgramID, "colorValue");
		lightID = glGetUniformLocation(ProgramID, "lightPosition");
		cameraID = glGetUniformLocation(ProgramID, "cameraPosition");

		shadingTypeID = glGetUniformLocation(ProgramID, "shadingType");

		glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	}


	void RenderManager::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		/****** Shader Program Bind Start ******/
		glUseProgram(ProgramID);


		CAGLE::ResourceManager& gResourceManager = CAGLE::ResourceManager::getInstance();

		/** View and Projection Matrix */
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, gResourceManager.getCamera("camera1")->loadViewMatrix());
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, gResourceManager.getCamera("camera1")->loadProjectionMatrix());

		/** Camera and Light position */
		glUniform4f(cameraID, gResourceManager.getCamera("camera1")->X(), gResourceManager.getCamera("camera1")->Y(), gResourceManager.getCamera("camera1")->Z(), 0);
		glUniform4f(lightID, gResourceManager.getLight()->X(), gResourceManager.getLight()->Y(), gResourceManager.getLight()->Z(), 0);

		/** Shading Type, I supported phong and gouroud */
		glUniform1i(shadingTypeID, shadingType);//shadingType);


		glEnableVertexAttribArray(vertexPositionID);
		glEnableVertexAttribArray(normalID);

		/* Each object*/

		for (const auto& each : gResourceManager.get_all_objects())
		{
			renderObject(each.second);
		}


		/* Terrain render */
		renderTerrain();


		glDisableVertexAttribArray(vertexPositionID);
		glDisableVertexAttribArray(normalID);


		glUseProgram(0);
		/****** Shader Program Bind End *****/

		glutSwapBuffers();
	}


	void RenderManager::renderTerrain()
	{
		auto& gResourceManager = CAGLE::ResourceManager::getInstance();

		glColor3f(0.3f, 0.9f, 0.0f);
		for (int z = 0; z < gResourceManager.getTerrain()->Length() - 1; z++) {
			//Makes OpenGL draw a triangle at every three consecutive vertices
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < gResourceManager.getTerrain()->Width(); x++) {

				glVertex3f(x / 10.f, gResourceManager.getTerrain()->Height(x, z), z / 10.f);
				glVertex3f(x / 10.f, gResourceManager.getTerrain()->Height(x, z + 1), (z + 1) / 10.f);
			}
			glEnd();
		}
	}


	void RenderManager::renderObject(CAGLE::Object *object)
	{

		/** Model Matrix */
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, object->loadModelMatrix());

		/** Color */
		glUniform4f(colorID, 1.f* (object->Color() / 0x10000) / 0xFF,
			1.f* (object->Color() / 0x100 % 0x100) / 0xFF,
			1.f* (object->Color() % 0x100) / 0xFF,
			0.0f
		);


		/** Vertex */
		glVertexAttribPointer(
			vertexPositionID,// The attribute we want to configure
			3,    // size
			GL_FLOAT,// type
			GL_FALSE,// normalized?
			sizeof(GLfloat) * 3,// stride
			object->loadVertexPosition()// array buffer offset
		);


		/** Normal Vector */
		glVertexAttribPointer(
			normalID,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GLfloat) * 3,
			object->loadNormal()
		);




		/** Draw call */
		glDrawArrays(GL_TRIANGLES, 0, object->loadPolygonCount() * 3);
	}
}