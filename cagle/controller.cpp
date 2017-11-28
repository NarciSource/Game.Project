#include "controller.h"

namespace CAGLE {
	
	Controller::Controller(int num, Player* player) :num(num), player(player)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}

	void Controller::handler()
	{
		if ((result = XInputGetState(num, &state)) == ERROR_SUCCESS)
		{
			thumbRight();
			thumbLeft();
		}
		else {

		}
	}

	void Controller::thumbRight()
	{
		/* camera look around */
		if (state.Gamepad.sThumbRX<-bound)
		{
			player->lookAround(CAGLE_LEFT, abs(state.Gamepad.sThumbRX)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbRX>bound)
		{
			player->lookAround(CAGLE_RIGHT, abs(state.Gamepad.sThumbRX)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbRY>bound)
		{
			player->lookAround(CAGLE_UP, abs(state.Gamepad.sThumbRY)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbRY<-bound)
		{
			player->lookAround(CAGLE_DOWN, abs(state.Gamepad.sThumbRY)*1.f / sensitivity);
		}
	}

	void Controller::thumbLeft()
	{
		/* camera move */
		if (state.Gamepad.sThumbLX<-bound)
		{
			player->move(CAGLE::Direct::Left, abs(state.Gamepad.sThumbLX)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbLX>bound)
		{
			player->move(CAGLE::Direct::Right, abs(state.Gamepad.sThumbLX)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbLY>bound)
		{
			player->move(CAGLE::Direct::Forward, abs(state.Gamepad.sThumbLY)*1.f / sensitivity);
		}
		if (state.Gamepad.sThumbLY<-bound)
		{
			player->move(CAGLE::Direct::Backward, abs(state.Gamepad.sThumbLY)*1.f / sensitivity);
		}
	}
}