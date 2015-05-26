/*************************************************************************
	MapDiscoverer  -
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef MAP_DISCOVERER_HPP
#define MAP_DISCOVERER_HPP

//-------------------------------------------------------- System includes
#include <list>
#include <pthread.h>

//-------------------------------------------------------- Personnal includes
#include "WorldMap.hpp"

//------------------------------------------------------------------ Types
class Sphero;

enum orientation
{
	TRIGO, HORAIRE
};

//------------------------------------------------------- Class definition
class MapDiscoverer
{
	public:
		//----------------------------------------------------- Operators

		MapDiscoverer& operator=(const MapDiscoverer&) = delete;

		//--------------------------------------- Constructors/Destructor

		MapDiscoverer(WorldMap *map);

		MapDiscoverer(const MapDiscoverer&) = delete;

		virtual ~MapDiscoverer();
		//------------------------------------------------ Public methods

		/**
		 * Les spheros doivent avoir leur orientation synchronisée et doivent
		 * être disposés de gauche à droite dans le sens d'ajout (le premier à
		 * partir sera le plus à gauche).
		 */
		void addSphero(Sphero* sphero);

	private:

		static void* SpheroThread(void* init);


		WorldMap *_world_map;

	class DiscoverAction
	{
		public:

			virtual ~DiscoverAction();

			virtual void effectuer(Sphero* sphero) = 0;
	};

	class ExploreLine : public DiscoverAction
	{
		public:
            ExploreLine(coord_t base, orientation orient, direction_t sens, MapDiscoverer* discoverer);

		public:
			virtual void effectuer(Sphero* sphero) = 0;

		private:
			coord_t _origine;
			direction_t _sens;
            orientation _orient;

            MapDiscoverer* _disc;
	};

	class OutlineExplore : public DiscoverAction
	{
		public:
			OutlineExplore(coord_t base, orientation orient, direction_t approche, MapDiscoverer* discoverer);

		public:
			virtual void effectuer(Sphero* sphero);

		private:
			coord_t _origine;
			orientation _orientation;
			direction_t _approche;

			MapDiscoverer* _disc;
	};





	list<DiscoverAction*> _actionList;
	list<pthread_t> _listPthread;
	pthread_mutex_t _mutexActions;
	pthread_cond_t _listActionsCond;
	static uint16_t _resolution;
};

#endif // MAP_DISCOVERER_HPP
