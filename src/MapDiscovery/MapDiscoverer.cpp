/*************************************************************************
	MapDiscoverer - 
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <sphero/Sphero.hpp>

//--------------------------------------------------------- Local includes
#include "MapDiscoverer.hpp"

//------------------------------------------------ Constructors/Destructor

MapDiscoverer::MapDiscoverer()
{
    sem_init(&_semaphoreLancement, 0, 1);
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

	sphero->enableCollisionDetection(70, 100, 70, 100, 50);

	sphero->setColor(0, 0xff, 0);

	while(!collision)
	{
		sphero->roll(80, 180);
	}
	
	sphero->setColor(0, 0xff, 0);

	collision = false;
	while(!collision)
	{
		sphero->roll(80, 270);
	}

	sphero->setColor(0, 0xff, 0);

    return (void*) NULL;
}
//Private classes
