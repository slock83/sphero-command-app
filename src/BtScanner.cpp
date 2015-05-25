/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "BtScanner.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QMenu>
#include <QDebug>

BtScanner::BtScanner(QWidget *parent)
	:   QDialog(parent), localDevice(new QBluetoothLocalDevice),
	  ui(new Ui_BtScanner)
{
	ui->setupUi(this);

	/*
	 * In case of multiple Bluetooth adapters it is possible to set adapter
	 * which will be used. Example code:
	 *
	 * QBluetoothAddress address("XX:XX:XX:XX:XX:XX");
	 * discoveryAgent = new QBluetoothDeviceDiscoveryAgent(address);
	 *
	 **/

	discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

	connect(ui->scan, SIGNAL(clicked()), this, SLOT(startScan()));

	connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
			this, SLOT(addDevice(QBluetoothDeviceInfo)));
	connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

	connect(ui->connect, SIGNAL(clicked(bool)), this, SLOT(connectSphero()));


	// add context menu for devices to be able to pair device
	ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayConnectMenu(QPoint)));

}

BtScanner::~BtScanner()
{
	delete discoveryAgent;
}

void BtScanner::addDevice(const QBluetoothDeviceInfo &info)
{
	QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
	QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);
	if (items.empty()) {
		QListWidgetItem *item = new QListWidgetItem(label);
		QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
		if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
			item->setTextColor(QColor(Qt::darkGreen));
		else
			item->setTextColor(QColor(Qt::black));

		ui->list->addItem(item);
	}
}

void BtScanner::startScan()
{
	ui->list->clear();
	discoveryAgent->start();
	ui->scan->setEnabled(false);
	ui->scanLbl->setText("Scan en cours...");
}

void BtScanner::scanFinished()
{
	ui->scanLbl->setText("Scan terminé");
	ui->scan->setEnabled(true);
}

void BtScanner::connectSphero()
{
	QListWidgetItem *currentItem = ui->list->currentItem();

	if(currentItem == NULL)
		return;

	QString text = currentItem->text();
	emit requestConnection(text);
}


void BtScanner::displayConnectMenu(const QPoint &pos)
{
	QMenu menu(this);
	QAction *connectAction = menu.addAction("Connecter");
	QAction *chosenAction = menu.exec(ui->list->viewport()->mapToGlobal(pos));
	QListWidgetItem *currentItem = ui->list->currentItem();

	QString text = currentItem->text();

	if (chosenAction == connectAction)
	{
		emit requestConnection(text);
	}
}
