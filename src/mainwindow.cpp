
#include <QString>
#include <QThread>
#include <QTimer>

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

	QObject::connect(_ch, SIGNAL(requestStatusBarUpdate(QString)), this, SLOT(setStatus(QString)), Qt::QueuedConnection);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
	timer->start(1000);

}

MainWindow::~MainWindow()
{
	delete ui;
	delete _ch;
}

void MainWindow::setStatus(QString status)
{
	_status = status;
}



void MainWindow::on_sendBtn_3_clicked()
{
	commandAction();
}

void MainWindow::on_commandLine_3_returnPressed()
{
	commandAction();
}

void MainWindow::updateStatus()
{
	ui->statusBar->showMessage(_status);
	update();
}

void MainWindow::commandAction()
{
	string cmdLine = ui->commandLine_3->text().toStdString();
	ui->commandLine_3->setText("");

	if(_ch->setParameter(cmdLine))
		_ch->start();
	else
		emit setStatus("A command is already running, please retry");
}
