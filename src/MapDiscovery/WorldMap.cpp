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

points_map_t WorldMap::getMap()
{
	return _points_map;
}

outlines_map_t WorldMap::getOutline()
{
	return _outlines_map;
}

bool WorldMap::addOutlinePolygonPoint(coord_t const& point)
{
	points_map_t::iterator it;
	point_struct_t* point_struct;

	polygon_t *polygon1 = nullptr, *polygon2 = nullptr;
	bool isFront1, isFront2;

	if((it = _points_map.find(point)) != _points_map.end())
	// Le point existe déjà, on l'a donc parcouru avant
		return false;

	point_struct = addPoint(point);

	// On recherche si le point en question est voisin d'une tête
	// ou d'une queue de polygone déjà existant

	for(polygon_t* polygon : _polygons_set)
	{
		if(point_struct->isNeighbour(polygon->front()))
		{
			// Le point est voisin de la tête d'un polygone
			if(polygon1 == nullptr)
			{
				polygon1 = polygon;
				isFront1 = true;
			}
			else
			{
				polygon2 = polygon;
				isFront2 = true;

				break;
			}
		}

		if(polygon->size() != 1 && point_struct->isNeighbour(polygon->back()))
		{
			// Le point est voisin de la queue d'un polygone
			if(polygon1 == nullptr)
			{
				polygon1 = polygon;
				isFront1 = false;
			}
			else
			{
				polygon2 = polygon;
				isFront2 = false;

				break;
			}
		}
	}

	if((polygon1 != nullptr) && (polygon2 != nullptr))
	{
		// On peut fusionner les deux listes

		if(polygon1 == polygon2)
		{
			// Si on a refermé un polygone
			polygon1->push_front(point);

			return true;
		}

		// Le point fait la liaison entre deux polygones non encore
		// fermés

		if(isFront1)
		// On mettra le nouveau point à la fin
			polygon1->reverse();

		polygon1->push_back(point);

		if(!isFront2)
			polygon2->reverse();

		// On remet à jour les points de polygon2
		// de manière à ce qu'ils soient associés à polygon1

		for(coord_t coord : *polygon2)
			_outlines_map.at(_points_map.at(coord)) = polygon1;

		// On ajoute le point dans le set
		_outlines_map.emplace(point_struct, polygon1);

		// On fait polygon1, polygon2

		polygon1->splice(polygon1->end(), *polygon2);
	}
	else if((polygon1 != nullptr) || (polygon2 != nullptr))
	{
		bool isFront = (polygon1 != nullptr) ? isFront1 : isFront2;
		polygon_t* polygon = (polygon1 != nullptr) ? polygon1 : polygon2;

		if(isFront)
			polygon->push_front(point);
		else
			polygon->push_back(point);

		// On ajoute le point dans le set
		_outlines_map.emplace(point_struct, polygon);
	}
	else
	{
		// Il faut créer un nouveau polygone
		polygon_t* new_polygon = new polygon_t();

		new_polygon->push_back(point);

		_outlines_map.emplace(point_struct, new_polygon);
		_polygons_set.emplace(new_polygon);
	}

	// le point n'existait pas, il a bien été ajouté
	return true;
}

bool WorldMap::existsOulinePolygonPoint(coord_t const& point)
{
	return _points_map.find(point) != _points_map.end();
}

bool WorldMap::isOutlinePolygonClosed(coord_t const& point)
{
	points_map_t::iterator it;

	point_struct_t* from;
	polygon_t* polygon;

	if((it = _points_map.find(point)) == _points_map.end())
		return false;

	polygon = _outlines_map.at(it->second);

	if(polygon->size() == 1 ||
		polygon->size() == 2)
		return false;

	from = _points_map.at(polygon->front());

	if(from->isNeighbour(polygon->back()))
		return true;

	return false;
}


point_struct_t* WorldMap::addPoint(coord_t const& point)
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

		if((it = _points_map.find(go(point, NORTH))) != _points_map.end())
		{
			// Un voisin existe au nord
			new_point->north = (*it).second;
			(*it).second->north = new_point;
		}

		if((it = _points_map.find(go(point, SOUTH))) != _points_map.end())
		{
			// Un voisin existe au sud
			new_point->south = (*it).second;
			(*it).second->south = new_point;
		}

		if((it = _points_map.find(go(point, EAST))) != _points_map.end())
		{
			// Un voisin existe à l'est
			new_point->east = (*it).second;
			(*it).second->east = new_point;
		}

		if((it = _points_map.find(go(point, WEST))) != _points_map.end())
		{
			// Un voisin existe à l'ouest
			new_point->west = (*it).second;
			(*it).second->west = new_point;
		}

		return new_point;
	}

	return nullptr;
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
