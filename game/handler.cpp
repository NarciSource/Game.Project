
#include <iostream>
#include "main.hpp"


bool flagRoverRemoteControl = false;
bool flagTimeFast = false;
float sensitivity = 1.f;
float sensitivityFrame = 30;

void mkarrow();
/**** no action, runtime ****/
void idle(int value)
{	
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();
	auto controller = gResourceManager.getController("controller1");
	if (controller != nullptr) {
		controller->handler();
		gResourceManager.adjustment();
	}

	/** Arrow action */
	for(auto each: gResourceManager.get_all_objects())
	{	
		auto obj1 = each.second;
		if (obj1->herNameIs().find("arrow", 0, 5) == std::string::npos) continue;
		std::string name = obj1->herNameIs();
		std::cout << name;
		bool flagCollison=false;

		/* If arrow is too far from the camera, erase it out. */
		if ((gResourceManager.getObject(name)->Position() - gResourceManager.getCamera("camera1")->Position()).length() >100) {
			gResourceManager.deleteObject(name);
			continue;
		}


		for(auto each2: gResourceManager.get_all_objects())
		{
			auto obj = each2.second;
			/* There is no effect between arrows */
			if (obj->herNameIs().find("arrow", 0, 5) != std::string::npos) continue;

			if (gResourceManager.isCollision(name, obj->herNameIs()))
			{
				flagCollison = true;
				break;
			}
		}
		/* Arrow does not collision anything */
		if (!flagCollison)
		{
			gResourceManager.getObject(name)->forward(sensitivity);
		}		
	}


	/** Tiger action **/
	for (int numoftigers = 0; numoftigers < 5; numoftigers++)
	{
		std::string name = "tiger" + std::to_string(numoftigers);

		if (gResourceManager.getObject(name) == nullptr) continue;

		if (name == "tiger0")
		{
			gResourceManager.getObject(name)->forward(sensitivity*1.f);
			gResourceManager.getObject(name)->around(sensitivity*1.f);
		}
		if (name == "tiger1")
		{
			gResourceManager.getObject(name)->forward(sensitivity*0.2f);
		}
		if (name == "tiger3")
		{
			gResourceManager.getObject(name)->forward(sensitivity*5.f);
			gResourceManager.getObject(name)->around(-sensitivity*2.f);
		}
		for(auto each: gResourceManager.get_all_objects())
		{
			auto obj = each.second;
			if (obj->herNameIs().find("arrow", 0, 5) != std::string::npos)
			{
				if (gResourceManager.isCollision(name, obj->herNameIs(),20))
				{
					gResourceManager.deleteObject(name);
					break;
				}
			}
			if (obj->hisNameIs() == "rover")
			{
				if (gResourceManager.isCollision(name, obj->hisNameIs(),40))
				{
					gResourceManager.deleteObject(name);
					break;
				}
			}
		}
	}


	/** Camera jump action effected on gravity */
	if (gResourceManager.getCamera("camera1")->Position().Y() > 10) //gravity
	{
	//	gResourceManager.getCamera("camera1")->move(CAGLE_DOWN, 0.03);
	}


	/** Light auto sunset effect */
	if (flagTimeFast)
	{
		gResourceManager.getLight()->Pitch(
			gResourceManager.getLight()->Pitch() + sensitivity*0.05f
		);
		gResourceManager.getLight()->sunset(sensitivity);
		printF(printLight);
	}

 	glutPostRedisplay();
	glutTimerFunc(sensitivityFrame, idle, 1);
}
/**** Handler: MouseClick ****/
void mouseClickHandler(const int button, const int state, const int x, const int y)
{
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();

	static int beforeFovy = gResourceManager.getCamera("camera1")->Fovy();

	/* When you hold down right mouse button,
	* show it at sniper style */
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) gResourceManager.getCamera("camera1")->Fovy(0.2);
	else {
		gResourceManager.getCamera("camera1")->Fovy(beforeFovy);
		beforeFovy = gResourceManager.getCamera("camera1")->Fovy();
	}

	/* When you click left button,
	* make an arrow on the spot.*/
	if (button == GLUT_LEFT_BUTTON) mkarrow();

	gResourceManager.getCamera("camera1")->shutter();
}
/**** Handler: Mouse moving ****/
void mouseMoveHandler(const int x, const int y)
{
	auto player = CAGLE::ResourceManager::getInstance().getPlayer("player1");

	int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;


	if (x < centerX)
		player->lookAround(CAGLE_LEFT, sensitivity);
	if (x > centerX)
		player->lookAround(CAGLE_RIGHT, sensitivity);
	if (y < centerY)
		player->lookAround(CAGLE_UP, sensitivity *1.5);
	if (y > centerY)
		player->lookAround(CAGLE_DOWN, sensitivity *1.5);

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2);
	
	printF(printCamera);
}
/**** Handler: Keyboard typing ****/
void keyboardHandler(const unsigned char key, const int x, const int y)
{
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();
	auto& gRenderManager = CAGLR::RenderManager::getInstance();
	auto player = gResourceManager.getPlayer("player1");
	static bool flagShit = true;


	switch (key)
	{
		/** Character Moving */
	case 'w':
		player->move(CAGLE::Direct::Forward, sensitivity); break;
	case 'W':
		player->move(CAGLE::Direct::Forward, sensitivity * 3); break;
	case 's':
		player->move(CAGLE::Direct::Backward, sensitivity); break;
	case 'S':
		player->move(CAGLE::Direct::Backward, sensitivity * 3); break;
	case 'a':
		player->move(CAGLE::Direct::Left, sensitivity); break;
	case 'A':
		player->move(CAGLE::Direct::Left, sensitivity * 3); break;
	case 'd':
		player->move(CAGLE::Direct::Right, sensitivity); break;
	case 'D':
		player->move(CAGLE::Direct::Right, sensitivity * 3); break;



		/** Romote rover object */
	case '8':
		flagRoverRemoteControl = true;
		gResourceManager.getObject("rover")->forward(sensitivity*0.4f);
		printF(printRemote); break;
	case '2':
		flagRoverRemoteControl = true;
		gResourceManager.getObject("rover")->forward(-sensitivity*0.4f);
		printF(printRemote); break;
	case '4':
		gResourceManager.getObject("rover")->around(-sensitivity*2.f );
		gResourceManager.getObject("rover")->forward(sensitivity*0.4f);
		printF(printRemote); break;
	case '6':
		gResourceManager.getObject("rover")->around( sensitivity*2.f);
		gResourceManager.getObject("rover")->forward(sensitivity*0.4f);
		printF(printRemote); break;


		/** Fovy changing */
	case ',':
		gResourceManager.getCamera("camera1")->fovyDown(10); break;
	case '.':
		gResourceManager.getCamera("camera1")->fovyUp(10); break;

	case 'p':
		gResourceManager.getCamera("camera1")->projectionSwitch(PROJECTION_PERSPECTIVE);	break;
	case 'o':
		gResourceManager.getCamera("camera1")->projectionSwitch(PROJECTION_ORTHOGONAL);		break;
	case 'm':
		gResourceManager.getCamera("camera1")->projectionSwitch(PROJECTION_MAPVIEW);		break;


		/** Shid down/up */
	case 'z':
		
//		if (flagShit) gResourceManager.getCamera("camera1")->move(CAGLE_DOWN, sensitivity);
//		else gResourceManager.getCamera("camera1")->move(CAGLE_UP, sensitivity);
		flagShit = !flagShit;
		break;
		/** Jump */
	case 32: // space bar = Jump
//		gResourceManager.getCamera("camera1")->move(CAGLE_UP, sensitivity * 3);
		break;
		
	case 13:
		glutFullScreen();
		std::cout << "wsize= "<<glutGet(GLUT_WINDOW_WIDTH) << " " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
		break;
		/** Auto sunset */
	case 't':
		flagTimeFast = !flagTimeFast;
		break;
		/** ESC */
	case 27:
		exit(0);
	}

	gResourceManager.adjustment();

	glutPostRedisplay();
}
/**** Handler: Special Keyboard ****/
void specialKeyboardHandler(const int key, const int x, const int y)
{
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();

	switch (key)
	{
	case GLUT_KEY_UP:
		gResourceManager.getLight()->Position(
			gResourceManager.getLight()->Position() + CAGLM::Vec3<float>(0, 0, -sensitivity*10)
		);
		break;
	case GLUT_KEY_DOWN:
		gResourceManager.getLight()->Position(
			gResourceManager.getLight()->Position() + CAGLM::Vec3<float>(0, 0, sensitivity*10)
		);
		break;
	case GLUT_KEY_PAGE_UP:
		if(sensitivity > 1) sensitivity++;
		else sensitivity *= 1.1f;
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (sensitivity > 1) sensitivity--;
		else sensitivity *= 0.9f;
		break;
	case GLUT_KEY_HOME:
		sensitivityFrame--;
		break;
	case GLUT_KEY_END:
		sensitivityFrame++;
		break;
	}
}
/**** Handler: Extend screen size ****/
void changeSize(int width, int height)
{
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();

	glViewport(0, 0, width, height);

	gResourceManager.getCamera("camera1")->Aspect(1.0f*width / height);
}








/**** Dynamic creation of arrows ****/
void mkarrow()
{
/*	auto arrow = gResourceManager.iWannaObject("arrow");
	/* Set of Arrow
	* to position and direction of Bow */
/*	arrow->Position(
		gResourceManager.getCamera("camera1")->getWeaponObject()->Position() + CAGLM::Vec3<float>(0, -2, -5)
	);
	arrow->Yaw(
		gResourceManager.getCamera("camera1")->getWeaponObject()->Yaw()
	);
	arrow->Size(5);
*/
	auto& gResourceManager = CAGLE::ResourceManager::getInstance();

	gResourceManager.refresh();
}