/*************************************************************************
	MapDiscoverer  -  
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef MAP_DISCOVERER_HPP
#define MAP_DISCOVERER_HPP

//-------------------------------------------------------- System includes
#include "Coord.hpp"


//------------------------------------------------------------------ Types

typedef std::unordered_map<coord_t, point_struct_t*> world_map_t;

//------------------------------------------------------- Class definition
class MapDiscoverer
{
	public:
		//----------------------------------------------------- Operators

		MapDiscoverer& operator=(const SpheroManager&) = delete;


		//--------------------------------------- Constructors/Destructor

		MapDiscoverer();

		MapDiscoverer(const MapDiscoverer&) = delete;

		virtual ~MapDiscoverer();


		//------------------------------------------------ Public methods


	private:
	
	world_map_t _map;
};

#endif // MAP_DISCOVERER_HPP
