
#include <pthread.h>
#include <QDebug>

#include "JoystickPlayer.h"
#include "JoystickAdaptor.h"

struct initStructure{
		Sphero* sph;
		JoystickAdaptor* adaptor;
		JoystickPlayer* player;
};

JoystickPlayer::JoystickPlayer(Sphero *sphero, int listenerIndex): played(sphero), _run(true), deadZone(20)
{
	joystick = new JoystickAdaptor(listenerIndex);
	joystick->start();
}

JoystickPlayer::~JoystickPlayer()
{
	delete joystick;
}

void JoystickPlayer::run()
{
	JoystickEvent lastEvent;
	lastEvent.type = NONE;

	int lastAngle = 0;
	int _SPEED = 100;
	bool isBackLedEnabled(false);

	while(_run)
	{
		JoystickEvent ev = joystick->next();
		if(ev.type == NONE)
			ev = lastEvent;

		if(ev.type == AXIS_1)
		{
			if(ev.buttonType == AXIS_X)
			{
				if(ev.value < deadZone && ev.value > -deadZone)
				{
					played->roll(0, lastAngle, 1);
					ev.type = NONE;
				}
				else if(ev.value > 0)
				{
					played->roll((uint8_t)_SPEED % 256 * (ev.value / 127), 90, 1);
					lastAngle = 90;
				}
				else
				{
					played->roll((uint8_t)_SPEED % 256 * (ev.value / 127), 270, 1);
					lastAngle = 270;
				}
			}
			else if (ev.buttonType == AXIS_Y)
			{
				if(ev.value < deadZone && ev.value > -deadZone)
				{
					played->roll(0, lastAngle, 1);
				}
				else if(ev.value < 0)
				{
					played->roll((uint8_t)_SPEED % 256 * (ev.value / 127), 0, 1);
					lastAngle = 0;
				}
				else
				{
					played->roll((uint8_t)_SPEED % 256 * (ev.value / 127), 180, 1);
					lastAngle = 180;
				}
			}
		}
		else if(ev.type == BUTTONS)
		{
			if(ev.buttonType == BUTTON_0 && ev.value == 1)
				played->setColor(0, 0, 255, false);
			else if(ev.buttonType == BUTTON_1 && ev.value == 1)
				played->setColor(255, 0, 0, false);
			else if(ev.buttonType == BUTTON_2 && ev.value == 1)
				played->setColor(255, 255, 0, false);
			else if(ev.buttonType == BUTTON_3 && ev.value == 1)
				played->setColor(0, 255, 0, false);
			else if(ev.buttonType == START && ev.value == 1)
			{

			}
			else if(ev.buttonType == SELECT && ev.value == 1)
			{
				if(isBackLedEnabled)
					played->setBackLedOutput(0);
				else
					played->setBackLedOutput(255);

				isBackLedEnabled = !isBackLedEnabled;
				ev.type = NONE;
			}
			else if(ev.buttonType == LEFT_PRESS)
			{
				if(ev.value == 0)
				{
					played->setHeading(0);
					lastAngle = 0;
					ev.type = NONE;
				}
				else
				{
					if(lastAngle < 2)
						lastAngle += 360;

					lastAngle -= 2;
					played->roll(0, lastAngle, 1);
				}
			}
			else if(ev.buttonType == RIGHT_PRESS)
			{
				if(ev.value == 0)
				{
					played->setHeading(0);
					lastAngle = 0;
					ev.type = NONE;
				}
				else
				{
					lastAngle += 2;
					if(lastAngle >= 360)
						lastAngle -= 360;
					played->roll(0, lastAngle, 1);
				}
			}
		}
		lastEvent = ev;
		usleep(8000);
	}
}

void JoystickPlayer::stop()
{
	joystick->stop();
	_run = false;
}

/*void* JoystickPlayer::commandSphero(void* initStruct)
{
	initStructure* init = (initStructure*)initStruct;

	JoystickAdaptor* adaptor = init->adaptor;
	Sphero* sphero = init->sph;
	JoystickPlayer* player = init->player;


}
*/
