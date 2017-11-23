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

		modelmatrix_uni_loc = glGetUniformLocation(ProgramID, "modelMatrix");
		viewmatrix_uni_loc = glGetUniformLocation(ProgramID, "viewMatrix");
		porjmatrix_uni_loc = glGetUniformLocation(ProgramID, "projectionMatrix");

		color_uni_loc = glGetUniformLocation(ProgramID, "colorValue");
		light_uni_loc = glGetUniformLocation(ProgramID, "lightPosition");
		cam_uni_loc = glGetUniformLocation(ProgramID, "cameraPosition");

		shadingtype_loc = glGetUniformLocation(ProgramID, "shadingType");

		vertex_attr_loc = glGetAttribLocation(ProgramID, "vertexPosition");
		if (vertex_attr_loc == -1) {
			std::cerr << "position" << std::endl;
		}


		normal_attr_loc = glGetAttribLocation(ProgramID, "vertexNormal");
		if (normal_attr_loc == -1) {
			std::cerr << "position" << std::endl;
		}

		glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

		defineVBO();
		defineVAO();

		delete[] vertex_vbo_name;
		delete[] normal_vbo_name;
	}


	void RenderManager::defineVBO()
	{
		vertex_vbo_name = new GLuint[gResMngr.sizeObjects()];
		normal_vbo_name = new GLuint[gResMngr.sizeObjects()];

		
		int i = 0;
		for (auto& each : gResMngr.get_all_objects())
		{
			auto obj = each.second;

			glGenBuffers(1, &vertex_vbo_name[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_name[i]);
			{
				glBufferData(GL_ARRAY_BUFFER,
					sizeof(GLfloat)*obj->PolygonCount() * 9,
					obj->Vertexs(),
					GL_STATIC_DRAW);
			}

			glGenBuffers(1, &normal_vbo_name[i]);
			glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_name[i]);
			{
				glBufferData(GL_ARRAY_BUFFER,
					sizeof(GLfloat)*obj->PolygonCount() * 9,
					obj->Normals(),
					GL_STATIC_DRAW);
			}
			i++;
		}
		sizeObjects = i;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void RenderManager::defineVAO()
	{
		object_vao_name = new GLuint[sizeObjects];


		for (unsigned int i = 0; i < sizeObjects; i++)
		{
			glGenVertexArrays(1, &object_vao_name[i]);
			glBindVertexArray(object_vao_name[i]);
			{
				glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_name[i]);
				{
					glVertexAttribPointer(vertex_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}

				glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_name[i]);
				{
					glVertexAttribPointer(normal_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
				glEnableVertexAttribArray(vertex_attr_loc);
				glEnableVertexAttribArray(normal_attr_loc);
			}
		}


		glBindVertexArray(0);
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

			glUniform4f(color_uni_loc, 1.f* (0xFFFFFF / 0x10000) / 0xFF,
				1.f* (0xFFFFFF / 0x100 % 0x100) / 0xFF,
				1.f* (0xFFFFFF % 0x100) / 0xFF,
				0.0f
			);

			glUniform1i(shadingtype_loc, 0x02);



			/** Object render */
			int i = 0;
			for (auto& each : gResMngr.get_all_objects())
			{
				renderObject(each.second, i++);
			}
			// clean bind
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(vertex_attr_loc);
			glEnableVertexAttribArray(normal_attr_loc);



			/** Ground render */
			renderGround(gResMngr.getGround("ground1"));



		}
		glUseProgram(0);
		glutSwapBuffers();
	}

	void RenderManager::renderObject(CAGLE::Object *obj, int num_object)
	{
		glUniformMatrix4fv(modelmatrix_uni_loc, 1, GL_FALSE, obj->ModelMatrix());

		glBindVertexArray(object_vao_name[num_object]);
		{
			glDrawArrays(GL_TRIANGLES, 0, obj->PolygonCount() * 3);
		}
	}

	void RenderManager::renderGround(CAGLE::Ground* ground)
	{

		/** Model Matrix */
		glUniformMatrix4fv(modelmatrix_uni_loc, 1, GL_FALSE, ground->ModelMatrix());

		/** Vertex */
		glVertexAttribPointer(
			vertex_attr_loc,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GLint) * 3,
			ground->Vertexs()
		);

		/** Normal */
		glVertexAttribPointer(
			normal_attr_loc,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GLint) * 3,
			ground->Normals()
		);

		glUniform4f(color_uni_loc, 1.f* (0x66CC66 / 0x10000) / 0xFF,
			1.f* (0x66CC66 / 0x100 % 0x100) / 0xFF,
			1.f* (0x66CC66 % 0x100) / 0xFF,
			0.0f
		);

		/** Draw call */
		glDrawElements(GL_TRIANGLE_STRIP, ground->indices_size(), GL_UNSIGNED_INT, ground->Indices());



		/** Color */
		glUniform4f(color_uni_loc, 1.f* (0xCC0000 / 0x10000) / 0xFF,
			1.f* (0xCC0000 / 0x100 % 0x100) / 0xFF,
			1.f* (0xCC0000 % 0x100) / 0xFF,
			0.0f
		);

		glDrawElements(GL_LINES, ground->indices_size(), GL_UNSIGNED_INT, ground->Indices());

	}
}