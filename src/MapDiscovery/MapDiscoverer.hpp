/*************************************************************************
	MapDiscoverer  -  
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef MAP_DISCOVERER_HPP
#define MAP_DISCOVERER_HPP

//-------------------------------------------------------- System includes
#include "WorldMap.hpp"

//------------------------------------------------------------------ Types

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


	private:
	
	WorldMap _world_map;
};

#endif // MAP_DISCOVERER_HPP
