#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

class CommandHandler;
#include "CommandHandler.h"
#include "BtScanner.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		void updateList();

	private slots:
		void on_sendBtn_3_clicked();

		void on_commandLine_3_returnPressed();

		void updateStatus();

		void on_actionConnect_triggered();

		void on_actionConnect_2_triggered();

	public slots:
		void setStatus(QString status);

	private:
		BtScanner bs;
		Ui::MainWindow *ui;
		QString _status;

		CommandHandler *_ch;

		void commandAction();
};

#endif // MAINWINDOW_H
