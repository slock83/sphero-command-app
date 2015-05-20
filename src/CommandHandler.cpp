
#include <string>
#include <sstream>
#include <QDebug>

using namespace std;


#include "CommandHandler.h"
#include "SpheroManager.h"

CommandHandler::CommandHandler(MainWindow *win): QThread(), _appWin(win), _cmdStream(NULL), _op(COMMAND)
{
	_sm = new SpheroManager(this);
}

CommandHandler::~CommandHandler()
{
	delete _sm;
}

bool CommandHandler::setParameter(string str, operation op)
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
		_op = op;
		_cmdLock.unlock();
		return true;
	}
	return false;
}


void CommandHandler::run()
{
	_cmdLock.lock();

	switch (_op)
	{
		case COMMAND:
		{
			string cmd;
			*_cmdStream >> cmd;
			handleCommand(cmd, *_cmdStream);
		}
			break;

		case CONNECT:
		{
			string addr, name;
			*_cmdStream >> addr >> name;
			getManager()->connectSphero(addr, name);
		}
			break;
		case DISCONNECT:
		{
			string name;
			*_cmdStream >> name;
			int index = _sm->getSpheroIndex(name);
			_sm->disconnectSphero(index);
		}
	}
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
	if(_sm->getSphero() == NULL || !_sm->getSphero()->isConnected())
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
		handleSelect(cmdStream);
	}
	else if(command == "disconnect")
	{
		handleDisconnect(cmdStream);
	}
	else
	{
		if(!isConnected())
		{
			setStatusBar("Please connect first");
			return;
		}

		if(command == "changecolor")
		{
			handleChangeColor(cmdStream);
		}
		else if(command == "backled")
		{
			handleBackled(cmdStream);
		}
		else if(command == "roll")
		{
			handleRoll(cmdStream);
		}
		else if(command == "head")
		{
			handleHead(cmdStream);
		}
		else if(command == "collision")
		{
			handleCollision(cmdStream);
		}
		else if(command == "setit")
		{
			handleInactivityTO(cmdStream);
		}
		else if(command == "reset")
		{
			setStatusBar("Reseting locator");
			_sm.getSphero()->configureLocator(0, 0, 0, 0);
		}
	}
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

void CommandHandler::handleChangeColor(stringstream &css)
{
	int r, g, b;
	css >> r >> g >> b;


	stringstream ss("");
	ss << "Changing Sphero color to : (" << r%256 << "," << g%256 << "," << b%256 << ")";


	setStatusBar(ss.str());

	_sm->getSphero()->setColor(r%256, g%256, b%256);
}

void CommandHandler::handleBackled(stringstream &css)
{
	int intensity;
	css >> intensity;


	stringstream ss("");
	ss << "Changing Sphero backled intensity to : " << intensity%256;


	setStatusBar(ss.str());

	_sm->getSphero()->setBackLedOutput(intensity%256);
}

void CommandHandler::handleCollision(stringstream &css)
{
	bool enable;
	css >> enable;

	if(enable)
	{
		int Xt, Xspd, Yt, Yspd, dead;
		css >> Xt >> Xspd >> Yt >> Yspd >> dead;
		setStatusBar("Collisions enabled");
		_sm->getSphero()->enableCollisionDetection(Xt, Xspd, Yt, Yspd, dead);
	}
	else
	{
		setStatusBar("Collisions disabled");
		_sm->getSphero()->disableCollisionDetection();
	}
}

void CommandHandler::handleRoll(stringstream &css)
{
	int speed, angle;
	css >> speed >> angle;

	stringstream ss("");
	ss << "Rolling at " << angle%360 << "° at speed " << speed;
	setStatusBar(ss.str());
	_sm->getSphero()->roll(speed, angle);
}

void CommandHandler::handleHead(stringstream &css)
{
	int angle;
	css >> angle;
	_sm->getSphero()->setHeading(angle);
}

void CommandHandler::handleInactivityTO(stringstream &css)
{
	int to;
	css >> to;

	stringstream ss("");
	ss << "Changing inactivity timeout time to " << to << "s";
	setStatusBar(ss.str());

	_sm->getSphero()->setInactivityTimeout(to);
}
