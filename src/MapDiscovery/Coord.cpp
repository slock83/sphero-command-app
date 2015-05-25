/*************************************************************************
	coord_t - 
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes
#include <unordered_map>

//--------------------------------------------------------- Local includes
#include "Coord.hpp"

//-------------------------------------------------------------- Operators

coord_t::coord_t()
	: x(0), y(0)
{
}

coord_t::coord_t(uint8_t _x, uint8_t _y)
	: x(_x), y(_y)
{
}

coord_t::coord_t(coord_t const& coord)
{
	x = coord.x;
	y = coord.y;
}

bool coord_t::operator==(coord_t const& coord) const
{
	return ((x == coord.x) && (y == coord.y));

}

coord_t& coord_t::operator=(coord_t const& coord)
{
	x = coord.x;
	y = coord.y;
	
	return *this;
}

namespace std
{
	std::size_t hash<coord_t>::operator()(coord_t const& coord) const
	{
		return hash<uint8_t>()(coord.x) ^ hash<uint8_t>()(coord.y);
	}
}