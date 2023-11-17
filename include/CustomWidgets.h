#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QtCore/qobject.h>
#include <qwidget.h>

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qboxlayout.h>

class VectorInput : public QWidget
{
	Q_OBJECT

private:
	QBoxLayout* layout;
	QLineEdit* xInput;
	QLineEdit* yInput;

public:
	enum Orientation {
		Horizontal,
		Vertical
	};

	VectorInput(Orientation);

};

class RGBInput : public QWidget
{
	Q_OBJECT

private:
	QSlider* rSlider;
	QSlider* gSlider;
	QSlider* bSlider;
	QSpinBox* rSpinBox;
	QSpinBox* gSpinBox;
	QSpinBox* bSpinBox;

public:
	RGBInput();
};





#endif
