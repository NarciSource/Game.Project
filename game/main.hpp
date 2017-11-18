#pragma once

#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"cagle.lib")
#pragma comment(lib,"caglr.lib")


#include "glew.h"
#include "glut.h"

#include "resourceManager.h"
#include "renderManager.h"

#include "object.h"
#include "model.h"



#define printCamera 0x0003
#define printLight 0x0004
#define printRemote 0x0005




extern int shadingType;


/** main.cpp */
void keyboardHandler(const unsigned char key, const int x, const int y);
void mouseMoveHandler(const int x, const int y);
void mouseClickHandler(const int button, const int state, const int x, const int y);
void specialKeyboardHandler(const int key, const int x, const int y);
void idle(int value);
void componentInit();
void printF(const int progress);
void changeSize(const int width, const int height);
