/*************************************************************************
	point_struct_t  -  
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef POINTSTRUCT_HPP
#define POINTSTRUCT_HPP

//-------------------------------------------------------- System includes
#include <stdint.h>

//------------------------------------------------------- Class definition
struct point_struct_t
{
	uint16_t x;
	uint16_t y;
	
	// Pointeurs vers chaque voisins du point courant
	point_struct_t* north;
	point_struct_t* east;
	point_struct_t* west;
	point_struct_t* south;
	
	point_struct_t();
};

#endif // POINTSTRUCT_HPP
