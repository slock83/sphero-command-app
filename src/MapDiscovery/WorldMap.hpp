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
#include "Polygon.hpp"

//------------------------------------------------------------------ Types

typedef std::unordered_map<coord_t, point_struct_t*> points_map_t;
typedef std::unordered_map<point_struct_t*, polygon_t*> outlines_map_t;
typedef std::unordered_set<polygon_t*> polygons_set_t;

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

		outlines_map_t getOutline();

		polygons_set_t getPolygons();


		//------------------------------------------------ Public methods

		/**
		 * @brief addOutlinePolygonPoint : Ajoute un point qui sera
		 *        ajouté à un polygone de contour.
		 *
		 */
		bool addOutlinePolygonPoint(coord_t const& point);

		/**
		 * @brief existsOulinePolygonPoint
		 * @param point
		 * @return
		 */
		bool existsOulinePolygonPoint(coord_t const& point);

		/**
		 * @brief isOutlinePolygonClosed
		 * @param point
		 * @return
		 */
		bool isOutlinePolygonClosed(coord_t const& point);

		/**
		 * @brief getLimitPoint : Renvoie la coordonnée limite à partir du
		 *		  point selon la direction passée en paramètre
		 * @param point
		 * @param direction
		 * @return
		 */
		coord_t getLimitPoint(coord_t const& point, int16_t direction);

		/**
		 * @brief addPoint : TODO
		 *
		 */
		point_struct_t* addPoint(coord_t const& point);

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

		// Contient les points qui forment les contours présents dans le
		// graphe et le polygone à qui ils appartiennent.
		// un polygone = un contour
		outlines_map_t _outlines_map;

		// Contient la liste des polygones
		polygons_set_t _polygons_set;
};

#endif // WORLDMAP_HPP
