#include "MapViewer.h"
#include "ui_MapViewer.h"

#include <QPainter>
#include <QTimer>
#include <map>

using namespace std;

MapViewer::MapViewer(QWidget *parent) :
	QWidget(parent), _map(NULL),
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

void MapViewer::setMap(WorldMap *map)
{
	_map = map;
}

void MapViewer::paintEvent(QPaintEvent *event)
{
	if(_map == NULL)
		return;
	int xmin(0), ymin(0), xmax(0), ymax(0);
	for(pair<coord_t, point_struct_t*> c : _map->getMap())
	{
		qDebug() << "Pass here";
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

	qDebug() << "Xmin " << xmin;
	qDebug() << "Ymin " << ymin;
	qDebug() << "Xmax " << xmax;
	qDebug() << "Ymax " << ymax;

	QPainter painter(this);
	painter.translate(-xmin, -ymin);

	if(xmax != xmin && ymax != ymin)
		painter.scale(width()/(1.0*(xmax-xmin)), height()/(1.0*(ymax-ymin)));

	QColor c(0, 0, 0);

	painter.setBrush(c);

	for(list<coord_t> *coordList: _map->getPolygons())
	{
		int len = coordList->size();
		QPoint points[len];
		int i = 0;
		for(coord_t c : *coordList)
		{
			qDebug() << c.x << " " << c.y;
			points[i] = QPoint(c.x, c.y);
			++i;
		}
		painter.drawConvexPolygon(points, len);
	}
}
