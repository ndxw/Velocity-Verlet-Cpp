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

	VectorInput(Orientation, QWidget*);

	bool isIncomplete();
	QString x();
	QString y();

signals:
	void textChanged(const QString&, const QString&);

private slots:
	void xChanged(const QString&);
	void yChanged(const QString&);

public slots:
	void clear();
	void setXText(const QString&);
	void setYText(const QString&);
};

class RGBInput : public QWidget
{
	Q_OBJECT

private:
	QString slider = QString(
		"QSlider::groove{\
			border: 1px solid white;\
			height: 3px;\
			margin: 0px 7px;}\
		QSlider::handle{\
			background-color: white;\
			width: 8px;\
			height: 8px;\
			margin: -5px;\
			border-radius: 6px;}\
		QSlider::add-page{\
			background-color: white;\
			border-radius: 2px;}\
		QSlider::sub-page{\
			border-radius: 2px;}\
		\
		QSlider#rSlider::handle{\
			border: 4px solid #ff0000;}\
		QSlider#rSlider::sub-page{\
			background-color: #ff0000;}\
		QSlider#gSlider::handle{\
			border: 4px solid #00ff00;}\
		QSlider#gSlider::sub-page{\
			background-color: #00ff00;}\
		QSlider#bSlider::handle{\
			border: 4px solid #0000ff;}\
		QSlider#bSlider::sub-page{\
			background-color: #0000ff;}\
		\
		QSlider#rSlider::handle{ \
			border: 4px solid #ff0000; }\
		QSlider#rSlider::sub-page{ \
			background-color: #ff0000; }\
		QSlider#gSlider::handle{ \
			border: 4px solid #00ff00; }\
		QSlider#gSlider::sub-page{ \
			background-color: #00ff00; }\
		QSlider#bSlider::handle{ \
			border: 4px solid #0000ff; }\
		QSlider#bSlider::sub-page{ \
			background-color: #0000ff; }");
	// gray out slider when disabled ^

	QString spinbox = QString(
		"QSpinBox{\
			width: 30px;\
			border: 1px inset gray;\
			border-radius: 2px;\
			padding-right: -15px;\
			margin-right: 20px;}\
		QSpinBox::up-button{\
			subcontrol-origin: margin;\
			subcontrol-position: top right;\
			width: 15px;\
			border: 1px outset gray;\
			border-top-left-radius: 7px;\
			border-top-right-radius: 7px;}\
		QSpinBox::down-button{\
			subcontrol-origin: margin;\
			subcontrol-position: bottom right;\
			width: 15px;\
			border: 1px outset gray;\
			border-bottom-left-radius: 7px;\
			border-bottom-right-radius: 7px;}");

	QGridLayout* rgbLayout;
	QSlider* rSlider;
	QSlider* gSlider;
	QSlider* bSlider;
	QSpinBox* rSpinBox;
	QSpinBox* gSpinBox;
	QSpinBox* bSpinBox;

public:
	RGBInput(QWidget*);

signals:
	void rValueChanged(int);
	void gValueChanged(int);
	void bValueChanged(int);

public slots:
	void setValues(int, int, int);
};


#endif