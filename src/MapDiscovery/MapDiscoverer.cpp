/*************************************************************************
	MapDiscoverer -
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <sphero/Sphero.hpp>
#include <unistd.h>
#include <QDebug>
#include <cmath>

//--------------------------------------------------------- Local includes
#include "MapDiscoverer.hpp"
#include "Coord.hpp"

//------------------------------------------------ Constructors/Destructor

uint16_t MapDiscoverer::_resolution = 60;

struct initializer{
		MapDiscoverer* disc;
		Sphero* sph;
};


MapDiscoverer::MapDiscoverer(WorldMap *map) : _world_map(map)
{
	pthread_mutex_init(&_mutexActions, NULL);
	_actionList.push_back(new OutlineExplore(coord_t(0,0),orientation::TRIGO, direction_t::NORTH, this));
}

MapDiscoverer::~MapDiscoverer()
{
	for(pthread_t pthread: _listPthread)
	{
		pthread_cancel(pthread);
	}
	for(pthread_t pthread : _listPthread)
	{
		pthread_join(pthread, NULL);
	}
	pthread_mutex_destroy(&_mutexActions);
}

//--------------------------------------------------------- Public methods
void MapDiscoverer::addSphero(Sphero* sphero)
{
	pthread_t threadId;

	initializer *init = new initializer();
	init->disc = this;
	init->sph = sphero;
	pthread_create(&threadId, NULL, SpheroThread, (void*)init);
	_listPthread.push_back(threadId);
}

//-------------------------------------------------------- Private methods
void* MapDiscoverer::SpheroThread(void* init){

	//--- Sphero Init ---//
	initializer* _init = (initializer*) init;
	Sphero* sphero = _init->sph;
	MapDiscoverer* disc = _init->disc;

	sphero->onPreSleep([sphero](){
				sphero->ping();
			});

	sphero->onDisconnect([sphero](){
				sphero->connect();
			});

	sphero->onCollision([&sphero](CollisionStruct*){
				sphero->roll(0,0);
				sphero->setColor(0xff, 0, 0);
				qDebug() << "Collision";
				//_world_map->addPoint(coord_t(cs->impact_component_x, cs->impact_component_y));
			});

	sphero->enableCollisionDetection(50, 5, 50, 5, 50);

	sphero->roll(60, 270);
	usleep(2000000);

	sphero->setColor(0, 0xff, 0);

	sphero->roll(60, 180);
	usleep(2000000);
	sphero->setColor(0, 0xff, 0);

	usleep(500000);

	// On s'éloigne un peu du mur
	int16_t x, y;
	x = sphero->getX() + 30;
	y = sphero->getY();
	sphero->rollToPosition(x, y);
	usleep(200000);
	sphero->rollToPosition(x, y);

	sleep(1);

	for(;;)
	{
		pthread_mutex_lock(&disc->_mutexActions);
		while(disc->_actionList.empty())
		{
			pthread_cond_wait(&disc->_listActionsCond, &disc->_mutexActions);
		}
		DiscoverAction* action = disc->_actionList.front();
		disc->_actionList.pop_front();
		pthread_mutex_unlock(&(disc->_mutexActions));
		action->effectuer(sphero);
		delete action;
	}

	return (void*) NULL;
}

//Private classes

MapDiscoverer::OutlineExplore::OutlineExplore(coord_t base, orientation orient,
															direction_t approche, MapDiscoverer* discoverer):
	_origine(base), _orientation(orient), _approche(approche), _disc(discoverer)
{
}

void MapDiscoverer::OutlineExplore::effectuer(Sphero* sphero)
{
	bool collision;
	int16_t lastX, lastY;
	int16_t lastCollisionX, lastCollisionY;
	int16_t newX, newY;
	int16_t direction = 0;

	switch(_approche)
	{
		case NORTH:
			direction = 0;
			break;
		case SOUTH:
			direction = 180;
			break;
		case EAST:
			direction = 90;
			break;
		case WEST:
			direction = 270;
			break;
	}

	qDebug() << "--- Détectons les contours Diégo ! ---";

	newX = _origine.x;
	newY = _origine.y;

	lastCollisionX = newX;
	lastCollisionY = newY;

	for(;;)
	{
		lastX = newX;
		lastY = newY;

		switch (direction)
		{
			case 0:
				newX = lastX;
				newY = lastY + _resolution;
				break;
			case 180:
				newX = lastX;
				newY = lastY - _resolution;
				break;
			case 90:
				newX = lastX + _resolution;
				newY = lastY;
				break;
			case 270:
				newX = lastX - _resolution;
				newY = lastY;
				break;
		}

		qDebug() << "go to " << newX << " " << newY;

		sphero->rollToPosition(newX, newY);

		collision = sphero->getCollision();
		usleep(200000);

		if(!collision)
		{
			sphero->rollToPosition(newX, newY);
			//collision = sphero->getCollision();
		}

		usleep(200000);

		if(collision)
		{
			lastCollisionX = lastX;
			lastCollisionY = lastY;

			// Sphero en orange
			sphero->setColor(255, 128, 128);

			qDebug() << " -> Point de collision trouvé";

			qDebug() << "go to " << lastX << " " << lastY;

			// On revient à la position initiale
			sphero->rollToPosition(lastX, lastY);

			usleep(200000);
			sphero->rollToPosition(lastX, lastY);

			if(!_disc->_world_map->addOutlinePolygonPoint(coord_t(lastX, lastY)))
			{
				qDebug() << "J'ai fini Diégo !";
				// On est retombé sur un point déjà visité
				return;
			}

			// Si on tourne dans le sens
			// horaire -> -90
			// trigo -> 90

			direction = (_orientation == orientation::HORAIRE) ?
						(direction + 270) % 360 : (direction + 90) % 360;

			// sphero en vert
			sphero->setColor(0, 255, 0);

			newX = lastX;
			newY = lastY;
		}
		else
		{
			qDebug() << "Point de non collision";
			direction = (_orientation == orientation::HORAIRE) ?
						(direction + 90) % 360 : (direction + 270) % 360;

			if(_disc->_world_map->existsOulinePolygonPoint(coord_t(newX, newY))
					&& (newX != lastCollisionX) || (newY != lastCollisionY))
			{
				// On est aussi retombé sur un point déjà visité
				qDebug() << "J'ai fini Vera";
				return;
			}
		}


	}

}

MapDiscoverer::ExploreLine::ExploreLine(coord_t base,orientation orient, direction_t sens, MapDiscoverer* discoverer):_origine(base),_orient(orient), _sens(sens), _disc(discoverer)
{
}

void MapDiscoverer::ExploreLine::effectuer(Sphero *sphero)
{

	int angle;
	switch (_sens)
	{
		case NORTH:
			angle = 0;
			break;
		case SOUTH:
			angle = 180;
			break;
		case EAST:
			angle = 90;
			break;
		case WEST:
			angle = 270;
			break;
	}

	do
	{

		coord_t dest = _disc->_world_map->getLimitPoint(_origine, angle);
		sphero->rollToPosition(dest.x, dest.y);
		if(sphero->getCollision() && (abs(sphero->getX() - dest.x) < 10) && (abs(sphero->getY() - dest.y) < 10))
		{
			_disc->_world_map->addOutlinePolygonPoint(coord_t(sphero->getX(), sphero->getY()));
			if(_orient == orientation::HORAIRE)
			MapDiscoverer::OutlineExplore(coord_t(sphero->getX(), sphero->getY()), orientation::TRIGO, _sens, _disc);
			else MapDiscoverer::OutlineExplore(coord_t(sphero->getX(), sphero->getY()), orientation::HORAIRE, _sens, _disc);
		}
	}while(true);


	sphero->roll(0, 0);
}


MapDiscoverer::DiscoverAction::~DiscoverAction()
{}
