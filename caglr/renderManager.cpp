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
		pushBuffer();
		glutDisplayFunc([] {
			RenderManager::getInstance().render();
		});
	}

	void RenderManager::info()
	{
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

		if (!GLEW_VERSION_4_3) {
			std::cerr << "OpenGL 4.3 API is not available." << std::endl;

		//	exit(EXIT_FAILURE);
		}
	}

	void RenderManager::setShadingType(ShadingType type)
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


		/* bind vga memory location */
		modelmatrix_uni_loc = glGetUniformLocation(ProgramID, "modelMatrix");
		viewmatrix_uni_loc = glGetUniformLocation(ProgramID, "viewMatrix");
		porjmatrix_uni_loc = glGetUniformLocation(ProgramID, "projectionMatrix");

		color_uni_loc = glGetUniformLocation(ProgramID, "colorValue");
		light_uni_loc = glGetUniformLocation(ProgramID, "lightPosition");
		cam_uni_loc = glGetUniformLocation(ProgramID, "cameraPosition");

		shadingtype_loc = glGetUniformLocation(ProgramID, "shadingType");

		vertex_attr_loc = glGetAttribLocation(ProgramID, "vertexPosition");

		normal_attr_loc = glGetAttribLocation(ProgramID, "vertexNormal");



		glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	}
	
	void RenderManager::pushBuffer()
	{
		/* render data make */
		for (auto& each : gResMngr.get_all_objects())
		{
			auto obj = each.second;

			buffers[obj]= new Buffer(obj, vertex_attr_loc, normal_attr_loc);
		}

		auto ground = gResMngr.getGround("ground1");
		buffers[ground] = new Buffer(ground, vertex_attr_loc, normal_attr_loc);
	}



	void RenderManager::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramID);
		{
			/** Common render 
			 cpu mem -> gpu mem */
			glUniformMatrix4fv(viewmatrix_uni_loc, 1, GL_FALSE, gResMngr.getCamera("camera1")->ViewMatrix());
			glUniformMatrix4fv(porjmatrix_uni_loc, 1, GL_FALSE, gResMngr.getCamera("camera1")->ProjMatrix());


			glUniform4f(cam_uni_loc, gResMngr.getCamera("camera1")->X(), gResMngr.getCamera("camera1")->Y(), gResMngr.getCamera("camera1")->Z(), 0);
			glUniform4f(light_uni_loc, gResMngr.getLight()->X(), gResMngr.getLight()->Y(), gResMngr.getLight()->Z(), 0);

			glUniform1i(shadingtype_loc, 0x02);


			/** object render */
			for (auto& each : gResMngr.get_all_objects())
			{
				auto obj = each.second;
				auto obj_buffer = buffers[obj];

				glUniformMatrix4fv(modelmatrix_uni_loc, 1, GL_FALSE, obj->ModelMatrix());

				glUniform4f(color_uni_loc, 1.f* (obj->Color() / 0x10000) / 0xFF,
					1.f* (obj->Color() / 0x100 % 0x100) / 0xFF,
					1.f* (obj->Color() % 0x100) / 0xFF,
					0.0f
				);

				glBindVertexArray(obj_buffer->Num());
				{
					glDrawArrays(GL_TRIANGLES, 0, obj_buffer->Size());
				}
			}
			
			/** ground render */
			auto ground = gResMngr.getGround("ground1");
			auto obj_buffer = buffers[ground];
			glUniformMatrix4fv(modelmatrix_uni_loc, 1, GL_FALSE, ground->ModelMatrix());
			glUniform4f(color_uni_loc, 1.f* (ground->Color() / 0x10000) / 0xFF,
				1.f* (ground->Color() / 0x100 % 0x100) / 0xFF,
				1.f* (ground->Color() % 0x100) / 0xFF,
				0.0f
			);
			glBindVertexArray(obj_buffer->Num());
			{
				glDrawElements(GL_TRIANGLE_STRIP, ground->indices_size(), GL_UNSIGNED_INT, ground->Indices());
			}


			/*// clean bind
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(vertex_attr_loc);
			glEnableVertexAttribArray(normal_attr_loc);
			*/
		}
		glUseProgram(0);
		glutSwapBuffers();
	}
}