
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
		handleSelect(cmdStream);
	}
	else if(command == "disconnect")
	{
		handleDisconnect(cmdStream);
	}
	else
	{
		if(!isConnected())
			setStatusBar("Please connect first");
		if(command == "coll")
		{
			/*	if(isConnected()) {
			CollisionStruct coll;
			sm.getSphero()->reportCollision(&coll);
		}*/
		}
		//------------------------------ Others
		else if(command == "changecolor")
		{
			handleChangeColor(cmdStream);
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
		_sm->getSphero()->enableCollisionDetection(Xt, Xspd, Yt, Yspd, dead);
	}
	else
		_sm->getSphero()->disableCollisionDetection();
}
