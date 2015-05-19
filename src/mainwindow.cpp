
#include <pthread.h>
#include <QString>
#include <QThread>

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
	commandAction();
}

void MainWindow::on_commandLine_3_returnPressed()
{
	commandAction();
}

void MainWindow::commandAction()
{
	string cmdLine = ui->commandLine_3->text().toStdString();

	if(_ch->setParameter(cmdLine))
		_ch->start();
	else
		setStatus("A command is already running, please retry", 2);
}
