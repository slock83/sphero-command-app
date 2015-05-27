#include "MapViewer.h"
#include "ui_MapViewer.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <map>
#include <QDebug>

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
	qDebug() << "Test1";
	if(_map == NULL)
		return;
	qDebug() << "Test2";
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
	painter.setPen(Qt::blue);
	painter.setFont(QFont("Arial", 30));
	painter.translate(-xmin, -ymin);

	/*if(xmax != xmin && ymax != ymin)
		painter.scale(width(), height());
	else
		painter.scale(width()/480, height()/480);*/


	QColor c(0, 0, 0);

	painter.setBrush(c);

	/*QPoint points[5];
	points[0] = QPoint(0, 480);
	points[1] = QPoint(0, 300);
	points[2] = QPoint(0, 360);
	points[3] = QPoint(0, 120);
	points[4] = QPoint(0, 540);

	painter.drawPolygon(points, 5);
	painter.setPen(QPen(Qt::black, 3));
	painter.drawPoint(QPoint(0, 480));
	painter.drawPoint(QPoint(10, 480));
	painter.drawPoint(QPoint(10, 50));

	painter.setBrush(c);*/

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
