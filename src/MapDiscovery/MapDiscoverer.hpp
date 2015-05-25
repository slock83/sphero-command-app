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
	
		/**
		 * @brief outlineDiscover : Découvre les contours d'une zone
		 *        non accessible à partir d'un certain point
		 * @param start_coord : la position initiale à partir de laquelle
		 *        il est nécessaire de chercher les contours.
		 * @param direction : la direction du sphero
		 *
		 */
		void outlineDiscoverer(coord_t start_coord, direction_t direction);
		
		
		WorldMap _world_map;
};

#endif // MAP_DISCOVERER_HPP
