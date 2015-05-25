#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QDialog>
#include <sphero/Sphero.hpp>

namespace Ui {
	class Calibrator;
}

class Calibrator : public QDialog
{
		Q_OBJECT

	public:
		explicit Calibrator(Sphero* sphero, int x, int y, int angle, QWidget *parent = 0);
		~Calibrator();

	private slots:
		void on_checkBox_clicked(bool checked);

		void on_sliderAngle_sliderMoved(int position);

		void on_sliderAngle_sliderReleased();

		void on_buttonBox_accepted();

		void on_buttonBox_rejected();

	private:
		Ui::Calibrator *ui;
		Sphero* sphero;
};

#endif // CALIBRATOR_H
