#pragma once
#pragma comment(lib,"XInput.lib")

#include <Windows.h>
#include <Xinput.h>

#include "player.h"


namespace CAGLE {
	class ResourceManager;
	class Controller {
	private:
		int num;
		XINPUT_STATE state;
		DWORD result;

		const int bound = 5000;
		const int sensitivity = 0x2000;
	private:

		Player* player;

	public:
		Controller(int num, Player* player);

		void handler();

	private:
		void thumbRight();

		void thumbLeft();
	};



}