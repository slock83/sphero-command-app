#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pthread.h>
#include <string>

using namespace std;

class CommandHandler;
#include "CommandHandler.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		void setStatus(const string& status, int timeout = 0);

	private slots:
		void on_sendBtn_3_clicked();

		void on_commandLine_3_returnPressed();

	private:
		Ui::MainWindow *ui;

		pthread_t _monitor;
		CommandHandler *_ch;

		void commandAction();
};

#endif // MAINWINDOW_H
