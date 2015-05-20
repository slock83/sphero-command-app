
#include <string>
#include <sstream>
#include <QDebug>

using namespace std;


#include "CommandHandler.h"
#include "SpheroManager.h"

CommandHandler::CommandHandler(MainWindow *win): QThread(), _appWin(win), _cmdStream(NULL)
{
	_sm = new SpheroManager(this);
}

CommandHandler::~CommandHandler()
{
	delete _sm;
}

bool CommandHandler::setParameter(string &str)
{
	if(_cmdStream != NULL)
	{
		delete _cmdStream;
		_cmdStream = NULL;
	}

	if(_cmdLock.tryLock(1))
	{
		_cmdStream = new stringstream("");
		*_cmdStream << str;
		_cmdLock.unlock();
		return true;
	}
	return false;
}

void CommandHandler::run()
{
	_cmdLock.lock();

	string cmd;
	*_cmdStream >> cmd;
	handleCommand(cmd, *_cmdStream);
	_cmdLock.unlock();
}

void CommandHandler::setStatusBar(string string)
{
	emit requestStatusBarUpdate(QString::fromStdString(string));
}

SpheroManager *CommandHandler::getManager()
{
	return _sm;
}

bool CommandHandler::isConnected()
{

    if(_sm->getSphero() == NULL)
	{
		stringstream ss("");
		ss << "Error : sphero not connected !";
		setStatusBar(ss.str());
		return false;
	}
	return true;
}

bool CommandHandler::lockListUpdate(bool state)
{
	if(state)
		return _listUpdateLock.tryLock(100);
	else
		_listUpdateLock.unlock();
	return true;
}


void CommandHandler::handleCommand(string& command, stringstream& cmdStream)
{
	transform(command.begin(), command.end(), command.begin(), ::tolower);

	//------------------------------ SpheroManager related
	if(command == "connect")
	{
		handleConnect(cmdStream);
	}
	else if(command == "select")
	{
		//		handleSelect(css);
	}
	else if(command == "disconnect")
	{
		handleDisconnect(cmdStream);
	}
	else if(command == "coll")
	{
		/*	if(isConnected()) {
			CollisionStruct coll;
			sm.getSphero()->reportCollision(&coll);
		}*/
	}
	//------------------------------ Others
	else if(command == "changecolor")
	{
		//	handleCc(css);
	}
	else if(command == "backled")
	{
		//	handleBackLED(css);
	}
	else if(command == "roll")
	{
		//	handleRoll(css);
	}
	else if(command == "head")
	{
		//	handleHead(css);
	}
	else if(command == "collision")
	{
		//	handleCollision();
	}
	else if(command == "ping")
	{
		//	ping();
	}
	else if(command == "setit")
	{
		//	handleIT(css);
	}
	else if(command == "sleep")
	{
		//	handleSleep(css);
	}
	else if(command == "exit")
	{
		/*		//To avoid memLeaks
		while(sm.getNbSpheros() > 0)
		{
			sm.disconnectSphero(0);
		}

		return 0;*/
	}
	else if(command == "read")
	{
		/*	if(!isConnected()) cout << "please connect first" <<endl;

		else sm.getSphero()->setDataStreaming(1, 1, 0, 0, mask2::ODOMETER_X | mask2::ODOMETER_Y);*/
	}
	else if(command == "reset")
	{
		/*	if(!isConnected()) cout << "please connect first" <<endl;

		else sm.getSphero()->configureLocator(0, 0, 0, 0);*/
	}
	else
	{

	}
	//	showHelp();

	//return -1;
}


void CommandHandler::handleConnect(stringstream &css)
{
	string address;
	css >> address;

	stringstream ss("");
	if(address == "")
		ss << "Trying to connect to last saved Sphero address";
	else
		ss << "Trying to connect to Sphero at address " << address;

	setStatusBar(ss.str());
	//_appWin->setStatus(ss.str());

	lockListUpdate(true);
	if(address == "")
		_sm->connectSphero(address, "Sphero");
	else
		_sm->connectSphero(address, address);
	lockListUpdate(false);
}


void CommandHandler::handleDisconnect(stringstream &css)
{
	int index;
	css >> index;


	stringstream ss("");
	if(index >= _sm->getNbSpheros())
	{
		ss << "Incorrect number";
		setStatusBar(ss.str());
		return;
	}
	ss << "Disconnecting Sphero number " << index;

	setStatusBar(ss.str());

	lockListUpdate(true);
	_sm->disconnectSphero(index);
	lockListUpdate(false);
}

void CommandHandler::handleSelect(stringstream& css)
{
	size_t idx;
	css >> idx;
	stringstream ss("");
    if(_sm->selectSphero(idx))
	{
		ss << "Selected sphero "<< idx;
		setStatusBar(ss.str());
		return;
	}
	ss << "error : no sphero known by id " << idx;
	setStatusBar(ss.str());
}

void CommandHandler::handleSleep(stringstream& css)
{
	if(!isConnected()) return;

	unsigned int time;
	css >> time;
    _sm->getSphero()->sleep((uint16_t) time);
	stringstream ss("");
	ss << "sphero sent to sleep for "<< time << " seconds";
	setStatusBar(ss.str());
    _sm->getSphero()->disconnect();
	/**
	* sleep(time+3);
	* sm.getSphero()->connect();
	**/
}
