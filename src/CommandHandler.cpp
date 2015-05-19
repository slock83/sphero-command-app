
#include "CommandHandler.h"
#include "SpheroManager.h"

CommandHandler::CommandHandler(MainWindow *win): _appWin(win)
{
	_sm = new SpheroManager(win);
}

CommandHandler::~CommandHandler()
{
	delete _sm;
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

	stringstream ss("Trying to connect to Sphero at address ");
	ss << address;

	_appWin->setStatus(ss.str());
	_sm->connectSphero(address);
}

void CommandHandler::handleDisconnect(stringstream &css)
{
	int index;
	css >> index;

	stringstream ss("Disconnecting Sphero number ");
	ss << index;
	_appWin->setStatus(ss.str());
	_sm->disconnectSphero(index);
}

