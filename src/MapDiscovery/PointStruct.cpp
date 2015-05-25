/*************************************************************************
	point_struct_t - 
							 -------------------
	started                : 25/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes

//--------------------------------------------------------- Local includes
#include "PointStruct.hpp"

//-------------------------------------------------------------- Operators

point_struct_t::point_struct_t()
	: x(0), y(0),
	north(nullptr), east(nullptr), west(nullptr), south(nullptr)
{
}

bool point_struct_t::isNeighbour(point_struct_t const& point)
{
	return isNeighbour(coord_t(point.x, point.y));
}

bool point_struct_t::isNeighbour(coord_t const& point)
{
	if(x == point.x)
	{
		if(y == (point.y - 1) 
			|| y == (point.y + 1))
			return true;
			
		return false;
	}
	
	if(y == point.y)
	{
		if(x == (point.x - 1) 
			|| x == (point.x + 1))
			return true;
	}
	
	return false;
}