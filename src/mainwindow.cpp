
#include <QString>
#include <QThread>
#include <QTimer>
#include <QFont>
#include <QMenu>
#include <QAction>

#include <QDebug>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

#include "CommandHandler.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BtScanner.h"
#include "MapDiscovery/MapDiscoverer.hpp"

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

	_joystickList = new QMenu("Connect joystick");
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

void MainWindow::updateJoysticks()
{
	int nbJoysticks;
	system("ls /dev/input/ | grep js | wc -l > ./tmp");

	ifstream inputList ("tmp", ios::in);
	if (inputList.is_open())
	{
		inputList >> nbJoysticks;
		inputList.close();
	}

	int jsIndexs[nbJoysticks];
	system("ls /dev/input/ | grep js > ./tmp");

	ifstream jsList ("tmp", ios::in);
	if (jsList.is_open())
	{
		for(int i = 0; i < nbJoysticks; ++i)
		{
			int nb;
			jsList >> nb;
			jsIndexs[i] = nb;
		}
		jsList.close();
	}

	system("rm ./tmp");

	_joystickList->clear();
	for(int i = 0; i < nbJoysticks; ++i)
	{
		stringstream ss("");
		ss << "Manette " << jsIndexs[i];
		QAction* action = _joystickList->addAction(QString::fromStdString(ss.str()));
		if(joystickBindings.find(jsIndexs[i]) != joystickBindings.end())
			action->setEnabled(false);
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

void MainWindow::updateConnexions(Sphero* sph)
{
	for(pair<int, Sphero*> p : joystickBindings)
	{
		if(sph == p.second)
			joystickBindings.erase(joystickBindings.find(p.first));
	}
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
	setStatus("Tentative de connexion au Sphero sélectionné...");

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
		emit setStatus("Une commande est en cours d'exécution, veuillez réessayer");
}

void MainWindow::on_actionConnect_2_triggered()
{
	_btScan->show();
}

void MainWindow::customContextMenuRequested(const QPoint &pos)
{
	updateJoysticks();
	QMenu menu(this);
	QAction *disconnectAction = menu.addAction("Déconnecter le Sphero");
	QAction *discJsAction = menu.addAction("Déconnecter les manettes");
	menu.addSeparator();
	QAction *streamAction = menu.addAction("Voir les infos de position");
    QAction *discoverAction = menu.addAction("Rejoindre Dora");
	menu.addMenu(_joystickList);

	QAction *chosenAction = menu.exec(ui->spheroLst->viewport()->mapToGlobal(pos));
	QListWidgetItem *currentItem = ui->spheroLst->currentItem();

	if(currentItem == NULL || chosenAction == NULL)
		return;
	QString text = currentItem->text();
	QString actionText = chosenAction->text();

	if(chosenAction == disconnectAction)
	{
		string spheroName = text.toStdString();

		if(_ch->setParameter(spheroName, operation::DISCONNECT))
			_ch->start();
		else
			emit setStatus("Une commande est en cours d'exécution, veuillez réessayer");
	}
	else if(chosenAction == streamAction)
	{
		if(_ch->setParameter(text.toStdString(), operation::TRACK))
			_ch->start();
		else
			emit setStatus("Une commande est en cours d'exécution, veuillez réessayer");
	}
	else if(chosenAction == discJsAction)
	{
		updateConnexions(_ch->getManager()->getSphero(text.toStdString()));
	}
    else if(chosenAction == discoverAction)
    {
        MapDiscoverer *map = new MapDiscoverer;
        map->addSphero(_ch->getManager()->getSphero(text.toStdString()));
    }
	else if(actionText.startsWith("Manette"))
	{
		stringstream ss("");
		ss << actionText.toStdString();
		int index;
		ss >> index;
		joystickBindings[index] = _ch->getManager()->getSphero(text.toStdString());
	}
}

void MainWindow::on_spheroLst_itemDoubleClicked(QListWidgetItem *item)
{
	int index = _ch->getManager()->getSpheroIndex(item->text().toStdString());
	_ch->getManager()->selectSphero(index);
	setStatus(QString("Selecting Sphero %1").arg(item->text()));
}
