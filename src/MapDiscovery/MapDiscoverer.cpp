/*************************************************************************
	MapDiscoverer -
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <sphero/Sphero.hpp>
#include <unistd.h>
#include <QDebug>

//--------------------------------------------------------- Local includes
#include "MapDiscoverer.hpp"

//------------------------------------------------ Constructors/Destructor

uint16_t MapDiscoverer::_resolution = 10;

struct initializer{
		MapDiscoverer* disc;
		Sphero* sph;
};


MapDiscoverer::MapDiscoverer(WorldMap *map) : _world_map(map)
{
	pthread_mutex_init(&_mutexActions, NULL);
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

	bool collision = false;

	sphero->onCollision([&sphero](CollisionStruct*){
				sphero->roll(0,0);
				sphero->setColor(0xff, 0, 0);
				qDebug() << "Collision";
				//_world_map->addPoint(coord_t(cs->impact_component_x, cs->impact_component_y));
			});

	sphero->enableCollisionDetection(60, 20, 60, 20, 60);

	sphero->roll(60, 270);
	usleep(200000);

	sphero->setColor(0, 0xff, 0);

	sphero->roll(60, 180);
	usleep(200000);
	sphero->setColor(0, 0xff, 0);

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

	int16_t lastX, lastY;
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

	for(;;)
	{
		lastX = sphero->getX();
		lastY = sphero->getY();

		switch (direction)
		{
			case 0:
				sphero->rollToPosition(lastX, lastY+5);
				break;
			case 180:
				sphero->rollToPosition(lastX, lastY-5);
				break;
			case 90:
				sphero->rollToPosition(lastX+5, lastY);
				break;
			case 270:
				sphero->rollToPosition(lastX-5, lastY);
				break;
		}

		if(sphero->getCollision())
		{
			// On revient à la position initiale
			sphero->rollToPosition(lastX, lastY);

			if(!_disc->_world_map->addOutlinePolygonPoint(coord_t(lastX, lastY)))
			{
				// On est retombé sur un point déjà visité
				return;
			}

			// Si on tourne dans le sens
			// horaire -> -90
			// trigo -> 90

			direction = (_orientation == orientation::HORAIRE) ?
						(direction + 270) % 360 : (direction + 90) % 360;
		}
		else
		{
			direction = (_orientation == orientation::HORAIRE) ?
						(direction + 90) % 360 : (direction + 270) % 360;
		}

		if(_disc->_world_map->existsOulinePolygonPoint(coord_t(lastX, lastY)))
		{
			// On est aussi retombé sur un point déjà visité
			return;
		}
	}

}

MapDiscoverer::ExploreLine::ExploreLine(coord_t base, direction_t sens):_origine(base), _sens(sens)
{
}

void MapDiscoverer::ExploreLine::effectuer(Sphero *sphero)
{
    switch (_sens)
    {
        case NORTH:
            sphero->roll(45, 0);
            break;
        case SOUTH:
            sphero->roll(45, 180);
            break;
        case EAST:
            sphero->roll(45,90);
            brak;
        case WEST:
            sphero->roll(45,270);
            break;
    }



    sphero->roll(0, 0);
}






























	
