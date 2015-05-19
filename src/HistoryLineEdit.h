/**

\file
\author Mattia Basaglia
\section License
This file is part of Knotter.
Copyright (C) 2012-2014  Mattia Basaglia
Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HISTORYLINEEDIT_HPP
#define HISTORYLINEEDIT_HPP

#include <QLineEdit>

class HistoryLineEdit : public QLineEdit
{
	Q_OBJECT

	int         current_line;
	QStringList lines;
	QString     unfinished;

public:
	explicit HistoryLineEdit(QWidget *parent = 0);

	/**
	 * @brief Number of lines
	 * @return Number of lines entered
	 */
	int lineCount() const { return lines.size(); }

private slots:
	void enter_pressed();

signals:
	/**
	 * @brief Emitted when some text is executed
	 */
	void lineExecuted(QString);

protected:
	void keyPressEvent(QKeyEvent *);
	void wheelEvent(QWheelEvent *);

	void previous_line();
	void next_line();

};

#endif // HISTORYLINEEDIT_HPP
