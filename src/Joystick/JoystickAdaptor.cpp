
#include <fstream>
#include <sstream>
#include <deque>

#include <QDebug>

using namespace std;

#include "JoystickAdaptor.h"


JoystickAdaptor::JoystickAdaptor(int joystickIndex): _read(true), _joystickIndex(joystickIndex)
{}


/**
 * @brief stop : Stops the listener
 */
void JoystickAdaptor::stop()
{
	_read = false;
}


/**
 * @brief next : Returns the next event into the events list
 * @return the event if there is one, NULL otherwise
 */
JoystickEvent JoystickAdaptor::next()
{
	_readerMutex.lock();

	if(_events.size() < 1)
	{
		_readerMutex.unlock();
		JoystickEvent ev;
		ev.type = NONE;
		return ev;
	}

	JoystickEvent ev = _events[0];
	_events.pop_front();

	_readerMutex.unlock();

	return ev;
}

void JoystickAdaptor::run()
{
	_events.clear();
	string fileLoc;


	stringstream ss("");
	ss << "/dev/input/js" << _joystickIndex;
	fileLoc = ss.str();

	ifstream joystickInput(fileLoc.c_str(), ios::in);

	if(joystickInput.is_open())
	{
		stop();
	}
	_read = true;

	while(_read)
	{
		for(int i = 0; i < 4; ++i)
			joystickInput.get();

		int data[4];
		for(int i = 0; i < 4; ++i)
			data[i] = joystickInput.get();

		JoystickEvent ev = interpret(data);

		if(ev.type != NONE)
			addEvent(ev);
	}

	joystickInput.close();
	exit();
}


/**
 * @brief addEvent : Adds an event to the queue
 * @param ev : The new event
 */
void JoystickAdaptor::addEvent(JoystickEvent ev)
{
	_readerMutex.lock();

	_events.push_back(ev);

	_readerMutex.unlock();
}


/**
 * @brief interpret : Interprets the data into an event
 * @param data : The data to interpret
 * @return the event interpreted
 */
JoystickEvent JoystickAdaptor::interpret(int *data)
{
	JoystickEvent ev;
	ev.type = NONE;

	switch(data[2])
	{
		case 1:
			ev.type = BUTTONS;
			ev.value = data[0];

			if(data[3] == 0)
				ev.buttonType = BUTTON_0 ;
			else if(data[3] == 1)
				ev.buttonType = BUTTON_1;
			else if(data[3] == 2)
				ev.buttonType = BUTTON_2;
			else if(data[3] == 3)
				ev.buttonType = BUTTON_3;
			else if(data[3] == 8)
				ev.buttonType = SELECT;
			else if(data[3] == 9)
				ev.buttonType = START;
			else if(data[3] == 4)
				ev.buttonType = LEFT_PRESS;
			else if(data[3] == 5)
					ev.buttonType = RIGHT_PRESS;

			break;
		case 2:
			ev.type = AXIS_1;

			if(data[0] == 0)
				ev.value = 0;
			else if (data[0] == 1)
				ev.value = -data[1];
			else
				ev.value = data[1];

			switch(data[3])
			{
				case 1:
					ev.buttonType = AXIS_Y;
					break;
				case 0:
					ev.buttonType = AXIS_X;
					break;
			}
			break;
	}
	return ev;
}
