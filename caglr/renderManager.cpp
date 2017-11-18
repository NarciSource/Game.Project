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
		glEnable(GL_LIGHTING);
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


		/* Ground render*/
		renderGround(gResourceManager.getGround("ground1"));


		glDisableVertexAttribArray(vertexPositionID);
		glDisableVertexAttribArray(normalID);


		glUseProgram(0);
		/****** Shader Program Bind End *****/

		glutSwapBuffers();
	}


	void RenderManager::renderGround(CAGLE::Ground* ground)
	{
		/** Model Matrix */
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, ground->loadModelMatrix());

		/** Vertex */
		glVertexAttribPointer(
			vertexPositionID,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GLint) * 3,
			ground->Vertexs()
		);

		/** Normal */
		glVertexAttribPointer(
			normalID,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GLint) * 3,
			ground->Normals()
		);

		glUniform4f(colorID, 1.f* (0x66CC66 / 0x10000) / 0xFF,
			1.f* (0x66CC66 / 0x100 % 0x100) / 0xFF,
			1.f* (0x66CC66 % 0x100) / 0xFF,
			0.0f
		);
		/** Draw call */
		glDrawElements(GL_TRIANGLE_STRIP, ground->indices_size(), GL_UNSIGNED_INT, ground->Indices());

		/** Color */
		glUniform4f(colorID, 1.f* (0xCC0000 / 0x10000) / 0xFF,
			1.f* (0xCC0000 / 0x100 % 0x100) / 0xFF,
			1.f* (0xCC0000 % 0x100) / 0xFF,
			0.0f
		);
		glDrawElements(GL_LINES, ground->indices_size(), GL_UNSIGNED_INT, ground->Indices());
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