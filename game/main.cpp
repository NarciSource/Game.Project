#include <iostream>
#include <fstream>
#include <string>



#include "main.hpp"
#include "json.h"

using Json = nlohmann::json;


void main(int argc, char* argv[]) {

	auto& gResourceManager =CAGLE::ResourceManager::getInstance();
	auto& gRenderManager =	CAGLR::RenderManager::getInstance(argc, argv);

	/** set components */
	componentInit();

	gRenderManager.info();

	gRenderManager.display();

	/******** Handler => handler.cpp **********/
	glutReshapeFunc(changeSize);	
//	glutIdleFunc(idle);
	glutTimerFunc(30, idle, 1);
	
	glutSpecialFunc(specialKeyboardHandler);
	glutMouseFunc(mouseClickHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	/****************************************/

//	ShowCursor(false);
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


	/** make object */
	for (auto it = save["o"].begin(); it != save["o"].end(); it++)
	{
		std::string name = it.key();
		Json list = it.value();


		CAGLE::ResourceManager::Type type;
		if (list["type"] == "ground")
			type = CAGLE::ResourceManager::Type::Ground;
		else if (list["type"] == "sky")
		{
			std::cout << "sky";
			type = CAGLE::ResourceManager::Type::Sky;
		}
		else
			type = CAGLE::ResourceManager::Type::Object;


		auto& object = gResourceManager.newObject(name, type);

		auto model = gResourceManager.newModel(list["fileName"], type);

		if (!list["texture"].is_null())
		{
			auto texture = gResourceManager.newTexture(list["texture"]);
			object.bind(texture);
		}

		object.bind(model);
		object.Size(list["size"]);
		object.Position(CAGLM::Vec3<float>(list["position"][0], list["position"][1], list["position"][2]));
	}


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

	auto& player = gResourceManager.newPlayer("player1");
	player.bind(gResourceManager.getCamera("camera1"));

	gResourceManager.newController("controller1", gResourceManager.getPlayer("player1"));
	gResourceManager.newKeyboard("keyboard1", gResourceManager.getPlayer("player1"));

	auto keyboard = gResourceManager.getKeyboard("keyboard1");
	keyboard->addHandler(static_cast<char>(13), [] {glutFullScreen(); });


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

