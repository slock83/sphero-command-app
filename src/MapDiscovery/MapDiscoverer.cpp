/*************************************************************************
	MapDiscoverer - 
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <sphero/Sphero.hpp>
#include <unistd.h>

//--------------------------------------------------------- Local includes
#include "MapDiscoverer.hpp"

static uint16_t const DEFAULT_RES = 10;

//------------------------------------------------ Constructors/Destructor

MapDiscoverer::MapDiscoverer():_resolution(DEFAULT_RES)
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
    pthread_create(&threadId, NULL, SpheroThread, (void*) sphero);
    _listPthread.push_back(threadId);
}

//-------------------------------------------------------- Private methods
void* MapDiscoverer::SpheroThread(void* sphero_ptr){

	//--- Sphero Init ---//
	Sphero* sphero = (Sphero*) sphero_ptr;
	sphero->onPreSleep([sphero](){
				sphero->ping();
			});

    sphero->onDisconnect([sphero](){
                sphero->connect();
            });

	bool collision = false;

	sphero->onCollision([&sphero, &collision](CollisionStruct*){
				sphero->roll(0,0);
				sphero->setColor(0xff, 0, 0);
				collision = true;
			});

    sphero->enableCollisionDetection(80, 20, 80, 20, 80);

	sphero->setColor(0, 0xff, 0);

	while(!collision)
	{
        sphero->roll(60, 180);
        usleep(200000);
	}
    usleep(500000);
	sphero->setColor(0, 0xff, 0);

	collision = false;
	while(!collision)
	{
        sphero->roll(60, 270);
        usleep(200000);
	}

    sphero->setColor(0, 0xff, 0);

    for(;;)
    {
        pthread_mutex_lock(&_mutexActions);
        while(_actionList.empty())
        {
            pthread_cond_wait(&_listActionsCond, &_mutexActions);
        }
        DiscoverAction action = _actionList.front();
        _actionList.pop_front();
        pthread_mutex_unlock(&_mutexActions);
        action.effectuer(sphero);
    }

    return (void*) NULL;
}

//Private classes

MapDiscoverer::OutlineExplore MapDiscoverer::OutlineExplore(coord_t base, orientation orientation,
                                                            direction_t approche):
    _origine(base), _orientation(orientation), _approche(approche)
{
}

void MapDiscoverer::OutlineExplore::effectuer(Sphero* sphero)
{
    spherocoord_t x,y, xObj, yObj;
    bool collision;

    sphero->onCollision([xObj, yObj, x, y, collision](){
                xObj = x;
                yObj = y;
                collision = true;
            });

    uint16_t angle;
    switch(_approche)
    {
        case direction_t::NORTH:
            angle = 0;
            break;
        case direction_t::SOUTH:
            angle = 180;
            break;
        case direction_t::EAST:
            angle = 90;
            break;
        case direction_t::WEST:
            angle = 270;
            break;
    }


    for(;;)
    {
        xObj = x = sphero->getX();
        yObj = y = sphero->getY();

        collision = false;

        switch(angle)
        {
            case 0:
                yObj += resolution;
                break;
            case 180:
                yObj -= resolution;
                break;
            case 90:
                xObj += resolution;
                break;
            case 270:
                xObj -= resolution;
                break;
        }

        while(xObj != sphero->getX() )
        {
            if(xObj < sphero->getX())
            {
                sphero->roll(65, 270);
            }
            else
            {
                sphero->roll(65,90);
            }
        }

        while(yObj != sphero->getY() )
        {
            if(yObj < sphero->getY())
            {
                sphero->roll(65, 180);
            }
            else
            {
                sphero->roll(65,0);
            }
        }

        uint16_t ajout;

        if(collision)
        {
            if(WorldMap.addOutlinePolygonPoint(coord_t(x/_resolution, y/_resolution)))
                return;
            ajout = (_orientation == orientation::TRIGO) ? 90 : 270;
        }
        else
        {
            ajout = (_orientation == orientation::TRIGO) ? 270 : 90;
        }

        angle = (angle + ajout)%360;
    }

}




































