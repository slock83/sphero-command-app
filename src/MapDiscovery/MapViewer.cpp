#include "MapViewer.h"
#include "ui_MapViewer.h"

MapViewer::MapViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MapViewer)
{
	ui->setupUi(this);
}

MapViewer::~MapViewer()
{
	delete ui;
}
