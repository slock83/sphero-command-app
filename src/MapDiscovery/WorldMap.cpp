/*************************************************************************
	WorldMap - 
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes

//--------------------------------------------------------- Local includes
#include "WorldMap.hpp"

//------------------------------------------------ Constructors/Destructor

WorldMap::WorldMap()
{
	
}

WorldMap::~WorldMap()
{
	
}

//--------------------------------------------------------- Public methods

void WorldMap::addOutlinePoint(point_struct_t* point)
{
	_outline_set.emplace(point);
}

void WorldMap::addPoint(coord_t const& point)
{
	points_map_t::iterator it;
	
	it = _points_map.find(point);
	
	if(it == _points_map.end())
	{
		// Le point n'existe pas encore dans la map, on l'ajoute
		
		point_struct_t* new_point = new point_struct_t();
		
		_points_map.emplace(point, new_point);
		
		// On met à jour le nouveau point et ses voisins (s'ils existent)
		
		new_point->x = point.x;
		new_point->y = point.y;
		
		
	}
}

coord_t WorldMap::go(coord_t const& point, direction_t direction)
{
	coord_t destination(point);

	if(direction == direction_t::NORTH)
		destination.y++;
	else if(direction == direction_t::SOUTH)
		destination.y--;
	else if(direction == direction_t::WEST)
		destination.x--;
	else if(direction == direction_t::EAST)
		destination.x++;
	
	return destination;
}

//-------------------------------------------------------- Private methods
