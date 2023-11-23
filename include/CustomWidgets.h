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