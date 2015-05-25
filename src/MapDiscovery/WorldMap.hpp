/*************************************************************************
	WorldMap  -
							 -------------------
	started                : 25/05/2015
*************************************************************************/

#ifndef WORLDMAP_HPP
#define WORLDMAP_HPP

//-------------------------------------------------------- System includes
#include <unordered_map>
#include <unordered_set>

//--------------------------------------------------------- Local includes
#include "Coord.hpp"
#include "PointStruct.hpp"

//------------------------------------------------------------------ Types

typedef std::unordered_map<coord_t, point_struct_t*> points_map_t;
typedef std::unordered_set<point_struct_t*> outline_set_t;

//------------------------------------------------------- Class definition
class WorldMap
{
	public:
		//----------------------------------------------------- Operators

		WorldMap& operator=(const WorldMap&) = delete;


		//--------------------------------------- Constructors/Destructor

		WorldMap();

		WorldMap(const WorldMap&) = delete;

		virtual ~WorldMap();

		points_map_t getMap();

		outline_set_t getOutline();


		//------------------------------------------------ Public methods

		/**
		 * @brief addOutlinePoint : TODO
		 *
		 */
		void addOutlinePoint(point_struct_t* point);

		/**
		 * @brief addPoint : TODO
		 *
		 */
		void addPoint(coord_t const& point);

		/**
		 * @brief getPoint : TODO
		 *
		 */
		coord_t go(coord_t const& point, direction_t direction);

		//--------------------------------------------- Public attributes

		// Contient l'association coordonnées <-> point_struct_t
		// Chaque point_struct_t possède sa position, et des pointeurs
		// vers chacun de ses voisins (si pas de voisin, nullptr)
		points_map_t _points_map;

		// Contient les points qui forment le contour du graphe
		// tous les points appartiennent à _points_map
		outline_set_t _outline_set;
};

#endif // WORLDMAP_HPP
