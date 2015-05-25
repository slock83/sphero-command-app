#include "Calibrator.h"
#include "ui_Calibrator.h"


#include <QDebug>

Calibrator::Calibrator(Sphero *sphero, int x, int y, int angle, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Calibrator), sphero(sphero)
{
	ui->setupUi(this);
	if(sphero == NULL)
		setVisible(false);

	ui->spinX->setValue(x);
	ui->spinY->setValue(y);
	ui->sliderAngle->setValue(angle);
	sphero->setBackLedOutput(255);
}

Calibrator::~Calibrator()
{
	delete ui;
}

void Calibrator::on_checkBox_clicked(bool checked)
{
	if(checked)
		sphero->setBackLedOutput(255);
	else
		sphero->setBackLedOutput(0);
}

void Calibrator::on_sliderAngle_sliderMoved(int position)
{
	if(position < 0)
		position += 360;

	sphero->roll(0, position, 1);
}

void Calibrator::on_sliderAngle_sliderReleased()
{
	sphero->setHeading(0);
	ui->sliderAngle->setValue(0);
}

void Calibrator::on_buttonBox_accepted()
{
	sphero->configureLocator(0, ui->spinX->value(), ui->spinY->value(), 0);
	sphero->setBackLedOutput(0);
}

void Calibrator::on_buttonBox_rejected()
{
	sphero->setBackLedOutput(0);
}

void Calibrator::on_sliderAngle_valueChanged(int value)
{
	on_sliderAngle_sliderMoved(value);
}

void Calibrator::on_pushButton_clicked()
{
	on_sliderAngle_sliderReleased();
}
