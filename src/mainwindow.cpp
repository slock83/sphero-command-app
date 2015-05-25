
#include <QString>
#include <QThread>
#include <QTimer>
#include <QFont>

#include <QDebug>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

#include "CommandHandler.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BtScanner.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_ch = new CommandHandler(this);
	_btScan = new BtScanner(this);

	QObject::connect(_ch, SIGNAL(requestStatusBarUpdate(QString)), this, SLOT(setStatus(QString)));
	QObject::connect(_btScan, SIGNAL(requestConnection(QString)), this, SLOT(connectSphero(QString)));

	ui->spheroLst->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(ui->spheroLst, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
	timer->start(100);

	updateInformations(0, 1, 2, 3, 6);

}

MainWindow::~MainWindow()
{
	delete ui;
	delete _ch;
	delete _btScan;
}

void MainWindow::updateList()
{
	if(_ch->lockListUpdate(true))
	{
		vector<string> *names = _ch->getManager()->listSpheros();
		QListWidgetItem *cwi = ui->spheroLst->currentItem();

		string selected = (cwi == NULL) ? "" : cwi->text().toStdString();
		int newIndex = _ch->getManager()->getSpheroIndex(selected);
		int sel = _ch->getManager()->getSelectedIndex();
		_ch->lockListUpdate(false);

		ui->spheroLst->clear();
		for(string name : *names)
		{
			ui->spheroLst->addItem(QString::fromStdString(name));
		}

		if(newIndex >= 0 && ui->spheroLst->count() > 0)
			ui->spheroLst->item(newIndex)->setSelected(true);

		QListWidgetItem *wi = ui->spheroLst->item(sel);
		if(wi != NULL)
		{
			QFont font = wi->font();
			font.setBold(true);
			wi->setFont(font);
		}

	}
}

void MainWindow::setStatus(QString status)
{
	_status = status;
	updateList();
}

void MainWindow::updateInformations(int xPos, int yPos, int xSpd, int ySpd, int angle)
{
	ui->posLbl->setText(QString("(%1,%2)").arg(xPos).arg(yPos));
	ui->speedLbl->setText(QString("(%1,%2)").arg(xSpd).arg(ySpd));
	ui->angleLbl->setText(QString("%1°").arg(angle));
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
}

void MainWindow::connectSphero(QString spheroInfos)
{
    setStatus("Trying to connect to selected Sphero...");

	if(_ch->setParameter(spheroInfos.toStdString(), operation::CONNECT))
		_ch->start();
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

void MainWindow::on_actionConnect_2_triggered()
{
	_btScan->show();
}

void MainWindow::customContextMenuRequested(const QPoint &pos)
{
	QMenu menu(this);
	QAction *disconnectAction = menu.addAction("Disconnect");
	menu.addSeparator();
	QAction *streamAction = menu.addAction("View position infos");
	QAction *chosenAction = menu.exec(ui->spheroLst->viewport()->mapToGlobal(pos));
	QListWidgetItem *currentItem = ui->spheroLst->currentItem();

	QString text = currentItem->text();
	if(chosenAction == disconnectAction)
	{
		string spheroName = text.toStdString();

		if(_ch->setParameter(spheroName, operation::DISCONNECT))
			_ch->start();
		else
			emit setStatus("A command is already running, please retry");
	}
	else if(chosenAction == streamAction)
	{
		if(_ch->setParameter(text.toStdString(), operation::TRACK))
			_ch->start();
		else
			emit setStatus("A command is already running, please retry");
	}
}

void MainWindow::on_spheroLst_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	string oldSphero, newSphero;

	if(current == NULL)
		return;

	oldSphero = (previous == NULL) ? "" : previous->text().toStdString();
	newSphero = current->text().toStdString();

	stringstream ss("");
	ss << newSphero << " " << oldSphero;
	if(_ch->setParameter(ss.str(), operation::TRACK))
		_ch->start();
}
