#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QWidget>

#include "WorldMap.hpp"

namespace Ui {
	class MapViewer;
}

class MapViewer : public QWidget
{
		Q_OBJECT

	public:
		explicit MapViewer(QWidget *parent = 0);
		~MapViewer();

		void setMap(WorldMap* map);

	private:
		Ui::MapViewer *ui;
		WorldMap* _map;


		void paintEvent(QPaintEvent *event);
};

#endif // MAPVIEWER_H
