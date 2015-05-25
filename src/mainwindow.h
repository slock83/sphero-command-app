#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <map>

using namespace std;

class CommandHandler;
#include "CommandHandler.h"
#include "BtScanner.h"
#include "Joystick/JoystickPlayer.h"
#include "Calibrator.h"

namespace Ui {
	class MainWindow;
}

struct posInfos{
		int xPos;
		int yPos;
		int xSpd;
		int ySpd;
		int angle;

		posInfos()
		{
			xPos = 0;
			yPos = 0;
			xSpd = 0;
			ySpd = 0;
			angle = -361;
		}
};

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		void updateList();

		void updateJoysticks();

		void updateInformations(int xPos, int yPos, int xSpd, int ySpd, int angle);

		void updateConnexions(Sphero *sph);

		posInfos getPosition();

	private slots:
		void on_sendBtn_3_clicked();

		void on_commandLine_3_returnPressed();

		void updateStatus();
		void connectSphero(QString spheroInfos);
		void calibrator(int angle, int posX, int posY);

		void on_actionConnect_2_triggered();

		void customContextMenuRequested(const QPoint &pos);

		void on_spheroLst_itemDoubleClicked(QListWidgetItem *item);

		void on_actionCalibrer_triggered();

	public slots:
		void setStatus(QString status);


	private:
		BtScanner *_btScan;
		Ui::MainWindow *ui;
		QString _status;

		CommandHandler *_ch;

		Calibrator* _calibrator;

		QMenu *_joystickList;
		map<int, Sphero*> joystickBindings;
		map<int, JoystickPlayer*> joystickAdaptorBindings;

		posInfos _spheroPos;
		QMutex _posMutex;

		void commandAction();
};

#endif // MAINWINDOW_H
