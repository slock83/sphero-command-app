#ifndef JOYSTICKPLAYER_H
#define JOYSTICKPLAYER_H

#include <sphero/Sphero.hpp>
#include "JoystickAdaptor.h"

#include <QThread>

class JoystickPlayer : public QThread
{
	public:
		JoystickPlayer(Sphero* sphero, int listenerIndex);
		virtual ~JoystickPlayer();

		void stop();

	private:
		Sphero* played;
		JoystickAdaptor* joystick;
		bool _run;
		int deadZone;

		pthread_t _player;

		void run();

		//static void* commandSphero(void* initStruct);
};

#endif // JOYSTICKPLAYER_H
