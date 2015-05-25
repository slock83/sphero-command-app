/*************************************************************************
	coord_t  -  
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef COORD_HPP
#define COORD_HPP

//-------------------------------------------------------- System includes
#include <functional>
#include <iostream>

#include <stdint.h>

//------------------------------------------------------- Class definition
struct coord_t
{
	uint16_t x;
	uint16_t y;
	
	coord_t();
	coord_t(uint16_t x, uint16_t y);
	coord_t(coord_t const& coord);
	
	bool operator==(coord_t const& coord) const;
	coord_t& operator=(coord_t const& coord);
};

namespace std
{
	template <>
	struct hash<coord_t>
	{
		public :
		
		std::size_t operator()(coord_t const& coord) const;
	};
}

#endif // COORD_HPP
