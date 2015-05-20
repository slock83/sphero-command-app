#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

// Qt includes

#include <QThread>
#include <QMutex>

//System includes

#include <string>
#include <sstream>

// Local includes
#include "SpheroManager.h"
#include "mainwindow.h"

//Namespace
using namespace std;


//Classes
class MainWindow;
class SpheroManager;


//Command handler
class CommandHandler : public QThread
{
		Q_OBJECT

	public:
		CommandHandler(MainWindow* win);

		virtual ~CommandHandler();

		/**
		 * @brief setParameter sets the command string
		 * @param str command to run
		 * @return false if string was not added to the stream
		 */
		bool setParameter(string &str);


		void run() Q_DECL_OVERRIDE;

		/**
		 * @brief setStatusBar updates the status bar
		 * @param string new status to display
		 */
		void setStatusBar(string string);

		SpheroManager* getManager();

		bool lockListUpdate(bool state);

	signals:

		/**
		 * @brief requestStatusBarUpdate ask the status bar to update
		 */
		void requestStatusBarUpdate(QString);

	private:

		MainWindow* _appWin;
		SpheroManager* _sm;

		QMutex _cmdLock;
		QMutex _listUpdateLock;

		stringstream *_cmdStream;

        void isConnected();

	protected:


		/**
		 * @brief handleCommand : handles a command for sphero
		 * @param command : The command
		 * @param cmdStream : The stream containing the command args
		 */
		void handleCommand(string& command, stringstream& cmdStream);

		/**
		 * @brief handleConnect connects to a sphero
		 * @param css stringstream with the parameter in 01:23:45:67:89 format
		 */
		void handleConnect(stringstream& css);

		/**
		 * @brief handleDisconnect disconnect a sphero
		 * @param css not really used here
		 */
		void handleDisconnect(stringstream& css);

		/**
		 * @brief handleSelect selects a connected sphero to become active
		 * @param css stringstream with the sphero id in it
		 */
		void handleSelect(stringstream& css);

		/**
		 * @brief handleChangeColor change the active sphero's color
		 * @param css stringstream containing the new color codes (int r, int g, int b)
		 */
		void handleChangeColor(stringstream& css);

		/**
		 * @brief handleBackled change the back led output of the active sphero
		 * @param css stringstream containing a value between 0 and 255
		 */
		void handleBackled(stringstream& css);

		/**
		 * @brief handleCollision enable the sphero's internal collision handling
		 * @param css stringstream containing a boolean in (1/0) or (true/false) format
		 */
		void handleCollision(stringstream& css);

		/**
		 * @brief handleColl simulate a collision
		 * @param css empty
		 */
		void handleColl(stringstream& css);

		/**
		 * @brief handleRoll sends a roll command to the active sphero
		 * @param css contains 0..255 (speed) 0..359 (angle)
		 */
		void handleRoll(stringstream& css);

		/**
		 * @brief handleHead changes the current sphero's heading
		 * @param css contains 0..359 (angle)
		 */
		void handleHead(stringstream& css);

		/**
		 * @brief handlePing sends a ping to the active sphero
		 * @param css empty
		 */
		void handlePing(stringstream& css);


		/**
		 * @brief handleSleep sends the active sphero to sleep.
		 * @param css contains the duration (in second) to sleep. /!\ might change
		 */
		void handleSleep(stringstream& css);

		/**
		 * @brief handleInactivityTO change the inactivity timeout of the current sphero
		 * @param css contains the new timeout (in second)
		 */
		void handleInactivityTO(stringstream& css);

		/**
		 * @brief handleReset resets the active sphero's locator
		 * @param css empty
		 */
		void handleReset(stringstream& css);
};

#endif // COMMANDHANDLER_H
