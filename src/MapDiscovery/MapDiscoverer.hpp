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
#include <sem.h>

//-------------------------------------------------------- Personnal includes
#include "WorldMap.hpp"

//------------------------------------------------------------------ Types
class Sphero;

enum class orientation
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

		MapDiscoverer();

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
	
        static void* SpheroThread(void* sphero_ptr);
		
		
		WorldMap _world_map;

	/*class DiscoverAction
	{
		protected:
			virtual void effectuer(Sphero* sphero) = 0;
	}

	class ExploreLine : public DiscoverAction
	{
		public:
			ExploreLine(coord_t base, direction_t sens);
			
		protected:
			final virtual effectuer(Sphero* sphero);

		private:
			coord_t _origine;
			direction_t _sens;
	}

	class OutlineExplore : public DiscoverAction
	{
		public:
			OutlineExplore(coord_t base, orientation orientation);

		protected:
			final virtual effectuer(sphero);

		private:
			coord_t _origine;
			orientation _orientation;
	}
	
	*/

    list<DiscoverAction> _actionList;
	list<pthread_t> _listPthread;
};

#endif // MAP_DISCOVERER_HPP
