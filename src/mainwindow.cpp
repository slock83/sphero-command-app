
#include <pthread.h>
#include <QString>

#include <string>
#include <sstream>

using namespace std;

#include "CommandHandler.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_ch = new CommandHandler(this);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete _ch;
}

void MainWindow::setStatus(const string& status, int timeout)
{
	this->statusBar()->showMessage(QString::fromStdString(status), timeout);
}



void MainWindow::on_sendBtn_3_clicked()
{
	pthread_create(&_monitor, NULL, commandAction, this);
}

void MainWindow::on_commandLine_3_returnPressed()
{
	pthread_create(&_monitor, NULL, commandAction, this);
}

void* MainWindow::commandAction(void *win)
{
	MainWindow* window = (MainWindow*) win;

	string cmdLine = window->ui->commandLine_3->text().toStdString();
	stringstream ss(cmdLine);

	string cmd;
	ss >> cmd;

	window->_ch->handleCommand(cmd, ss);

	return NULL;
}
