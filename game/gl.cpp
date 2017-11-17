#include <iostream>

#include "main.hpp"



GLuint ProgramID;
GLint modelMatrixID, viewMatrixID, projectionMatrixID;
GLint vertexPositionID,  normalID;
GLint colorID, lightID, cameraID;
GLint shadingTypeID;
GLuint texture;

char* vertexShaderName("shader\\VertexShader.vert");
char* fragmentShaderName("shader\\FragmentShader.frag");

void myGLInit()
{
	glEnable(GL_DEPTH_TEST);
	int glewtest = glewInit();
	if (glewtest != GLEW_OK) {
		std::cerr<<"glew error"<<std::endl;
	}

	ProgramID = CAGLR::LoadShaders(vertexShaderName, fragmentShaderName);

	modelMatrixID = glGetUniformLocation(ProgramID, "modelMatrix");
	viewMatrixID = glGetUniformLocation(ProgramID, "viewMatrix");
	projectionMatrixID = glGetUniformLocation(ProgramID, "projectionMatrix");

	vertexPositionID = glGetAttribLocation(ProgramID, "vertexPosition");
	normalID = glGetAttribLocation(ProgramID, "vertexNormal");

	colorID = glGetUniformLocation(ProgramID, "colorValue");
	lightID = glGetUniformLocation(ProgramID, "lightPosition");
	cameraID = glGetUniformLocation(ProgramID, "cameraPosition");

	shadingTypeID = glGetUniformLocation(ProgramID, "shadingType");

	/*
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //<7>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //<8>

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data); //<9>	//이미지 데이터

	texRec[i]->해제해도 됨.




	glEnable(GL_TEXTURE_2D);
	*/

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

	/*
	GLfloat fogColor[4] = {0.5f,0.5f,1.0f,1.0f};
	glFogi(GL_FOG_MODE, GL_LINEAR);     //
	glFogfv(GL_FOG_COLOR, fogColor);    // LINEAR모드의 안개 색 지정
	glFogf(GL_FOG_DENSITY, 0.3f);       // 안개의 밀도 지정.GL_EXP,GL_EXP2만 사용 
	glHint(GL_FOG_HINT, GL_NICEST);     // 멋있게 해달라구 요구
	glFogf(GL_FOG_START, 5.5f);         // LINEAR에서만 적용 안개가 보이는 Z값
	glFogf(GL_FOG_END, 7.0f);           // 안개가 들이워져서 물체가 보이지 않는 Z값
	glEnable(GL_FOG);                   // 안개 사용
	*/
}
void renderObject(CAGLE::Object *object)
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

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/****** Shader Program Bind Start ******/
	glUseProgram(ProgramID);


	
	/** View and Projection Matrix */
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, gManager.getCamera("camera1")->loadViewMatrix());
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, gManager.getCamera("camera1")->loadProjectionMatrix());

	/** Camera and Light position */
	glUniform4f(cameraID, gManager.getCamera("camera1")->X(), gManager.getCamera("camera1")->Y(), gManager.getCamera("camera1")->Z(), 0);
	glUniform4f(lightID, gManager.getLight()->X(), gManager.getLight()->Y(), gManager.getLight()->Z(), 0);

	/** Shading Type, I supported phong and gouroud */
	glUniform1i(shadingTypeID, shadingType);
	

	glEnableVertexAttribArray(vertexPositionID);
	glEnableVertexAttribArray(normalID);
	
	/* Each object*/

	for (const auto& each : gManager.get_all_objects())
	{
		renderObject(each.second);
	}


	glColor3f(0.3f, 0.9f, 0.0f);
	for (int z = 0; z < gManager.getTerrain()->Length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < gManager.getTerrain()->Width(); x++) {

			glVertex3f(x/10.f, gManager.getTerrain()->Height(x, z), z/10.f);
			glVertex3f(x/10.f, gManager.getTerrain()->Height(x, z + 1), (z+1)/10.f);
		}
		glEnd();
	}


	glDisableVertexAttribArray(vertexPositionID);
	glDisableVertexAttribArray(normalID);








	glUseProgram(0);
	/****** Shader Program Bind End *****/

	glutSwapBuffers();
}