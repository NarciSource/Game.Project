#pragma once

#include <map>
#include "hid.h"



namespace CAGLE {

	class Keyboard : public HumanInterfaceDevice {

	private:
		Player* player;

		std::map<char,void(*)(void)> userHadlers;

	public:
		Keyboard(Player* player) :player(player) {}

		void handler() {};

		void keyboardHandler(const unsigned char key, const int x, const int y)
		{
			const float sensitivity = 1.f;

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
			case 27:
				exit(0);
				break;
			default:
				for (auto each : userHadlers)
				{
					if (each.first == key)
						each.second();
				}
			}
		}

		void addHandler(char key, void(*userHandler)(void))
		{
			userHadlers[key]=userHandler;
		}
	};


}