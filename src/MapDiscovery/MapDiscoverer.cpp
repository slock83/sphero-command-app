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

//------------------------------------------------ Constructors/Destructor

MapDiscoverer::MapDiscoverer()
{
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

    return (void*) NULL;
}
//Private classes
