#include "MapViewer.h"
#include "ui_MapViewer.h"

#include <QPainter>

MapViewer::MapViewer(WorldMap *map, QWidget *parent) :
	QWidget(parent), _map(map),
	ui(new Ui::MapViewer)
{
	ui->setupUi(this);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
}

MapViewer::~MapViewer()
{
	delete ui;
}

void MapViewer::paintEvent(QPaintEvent *event)
{
	int xmin(0), ymin(0), xmax(0), ymax(0);
	for(pair<coord_t, point_struct_t*> c : _map->getMap())
	{
		coord_t coord = c.first;

		if(coord.x < xmin)
			xmin = coord.x;
		else if(coord.x > xmax)
			xmax = coord.x;


		if(coord.y < ymin)
			ymin = coord.y;
		else if(coord.y > ymax)
			ymax = coord.y;
	}

	QPainter painter(this);
	painter.translate(-xmin, -ymin);
	painter.scale(width()/(1.0*(xmax-xmin)), height()/(1.0*(ymax-ymin)));

	QColor c(0, 0, 0);

	painter.setBrush(c);

	for(pair<coord_t, point_struct_t*> c : _map->getMap())
	{
		coord_t coord = c.first;
		painter.drawPoint(coord.x, coord.y);
	}
}
