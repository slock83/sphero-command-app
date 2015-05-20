#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QThread>
#include <QMutex>

#include <string>
#include <sstream>

using namespace std;

class MainWindow;
class SpheroManager;

#include "SpheroManager.h"
#include "mainwindow.h"

class CommandHandler : public QThread
{
		Q_OBJECT

	public:
		CommandHandler(MainWindow* win);

		virtual ~CommandHandler();


		bool setParameter(string &str);

		void run() Q_DECL_OVERRIDE;

		void setStatusBar(string string);

		SpheroManager* getManager();

	signals:

		void requestStatusBarUpdate(QString);

	private:
		MainWindow* _appWin;
		SpheroManager* _sm;

		QMutex _cmdLock;

		stringstream *_cmdStream;

	protected:


		/**
		 * @brief handleCommand : handles a command for sphero
		 * @param command : The command
		 * @param cmdStream : The stream containing the command args
		 */
		void handleCommand(string& command, stringstream& cmdStream);


		void handleConnect(stringstream& css);
		void handleDisconnect(stringstream& css);
		void handleSelect(stringstream& css);

		void handleChangeColor(stringstream& css);
		void handleBackled(stringstream& css);

			//enabling/disabling
		void handleCollision(stringstream& css);
			//simulating
		void handleColl(stringstream& css);

		void handleRoll(stringstream& css);
		void handleHead(stringstream& css);
		void handlePing(stringstream& css);

		void handleSleep(stringstream& css);
		void handleInactivityTO(stringstream& css);

		void handleReset(stringstream& css);
};

#endif // COMMANDHANDLER_H
