#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

#include "main.hpp"
#include "json.h"

using Json = nlohmann::json;


void main(int argc, char* argv[]) {

	auto& gResourceManager =CAGLE::ResourceManager::getInstance();
	auto& gRenderManager =	CAGLR::RenderManager::getInstance(argc, argv);



	/** set components */
	componentInit();

	gRenderManager.display();



	

	/******** Handler => handler.cpp **********/
	glutReshapeFunc(changeSize);	
//	glutIdleFunc(idle);
	glutTimerFunc(30, idle, 1);
	glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(specialKeyboardHandler);
	glutMouseFunc(mouseClickHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	/****************************************/

	ShowCursor(false);
	glutMainLoop();
}


/****  Set Init position  ****/
void componentInit()
{
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();

	GLint	windowSizeX = 2048;
	GLint	windowSizeY = 1024;

	

	Json save;
	std::ifstream in("data\\data.json");
	in >> save;

	/** make Camera */
	for (auto it = save["c"].begin(); it != save["c"].end(); it++)
	{
		std::string name = it.key();
		Json list = it.value();

		auto& camera = gResourceManager.newCamera(name);

		camera.Aspect(windowSizeX / windowSizeY);
		camera.Position(CAGLM::Vec3<float>(list["position"][0], list["position"][1], list["position"][2]));
		camera.LookAt(CAGLM::Vec3<float>(list["lookat"][0], list["lookat"][1], list["lookat"][2]));
		camera.Far(list["far"]);
	}

	/** make object */
	for (auto it = save["o"].begin(); it != save["o"].end(); it++)
	{
		std::string name = it.key();
		Json list = it.value();

		auto& object = gResourceManager.newObject(name);
		auto model = gResourceManager.newModel(list["fileName"]);

		object.bind(model);
		object.Size(list["size"]);
		object.Position(CAGLM::Vec3<float>(list["position"][0], list["position"][1], list["position"][2]));
	}

	/** make Terrain */
	for (auto it = save["g"].begin(); it != save["g"].end(); it++)
	{
		std::string name = it.key();
		Json list = it.value();

		auto& ground = gResourceManager.newGround(name);
		auto model = gResourceManager.newTerrain(list["fileName"]);

		ground.bind(model);
		ground.Size(list["size"]);
		ground.Position(CAGLM::Vec3<float>(list["position"][0], list["position"][1], list["position"][2]));
		ground.refresh();
	}




	/** make Light */
	gResourceManager.iWannaLight();

	gResourceManager.getLight()->Position(CAGLM::Vec3<float>(40, 200, -50));
	
	gResourceManager.refresh();

	in.close();
}

/**** print string ****/
void printF(int progress)
{
	CAGLE::ResourceManager& gResourceManager = CAGLE::ResourceManager::getInstance();

	switch (progress)
	{
	case printCamera:
		std::cout << std::fixed;
		std::cout.precision(1);
		std::cout << "\r#Camera tracing: X=" << gResourceManager.getCamera("camera1")->X() << " Y=" << gResourceManager.getCamera("camera1")->Y() << " Z=" << gResourceManager.getCamera("camera1")->Z()
			<< " roll=" << gResourceManager.getCamera("camera1")->Roll() << " yaw=" << gResourceManager.getCamera("camera1")->Yaw() << " pitch=" << gResourceManager.getCamera("camera1")->Pitch()
			<< " fov=" << gResourceManager.getCamera("camera1")->Fovy()<<" aspect=" << gResourceManager.getCamera("camera1")->Aspect();
		break;
	case printLight:
		std::cout << std::fixed;
		std::cout.precision(1);
		std::cout << "\r#Light tracing: Y=" << gResourceManager.getLight()->Y() << " Z=" << gResourceManager.getLight()->Z() <<"\t\t\t\t\t\t\t\t\t";
		break;
	case printRemote:
		std::cout << std::fixed;
		std::cout.precision(1);
		std::cout << "\r#Remote tracing: X=" << gResourceManager.getObject("rover")->X() << " Z=" << gResourceManager.getObject("rover")->Z() << "\t\t\t\t\t\t\t\t\t";
		break;
	}
}

