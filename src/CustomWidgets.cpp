#include "../include/CustomWidgets.h"

#include <QtWidgets/qlabel.h>

VectorInput::VectorInput(Orientation orient, QWidget* parent) : QWidget(parent)
{
	if (orient == Orientation::Horizontal) { layout = new QHBoxLayout(this); }
	else { layout = new QVBoxLayout(this); }

	this->setLayout(layout);

	QDoubleValidator* doubleValidator = new QDoubleValidator(this);

	QHBoxLayout* xInputLayout = new QHBoxLayout(this);
	QHBoxLayout* yInputLayout = new QHBoxLayout(this);
	xInput = new QLineEdit(this);
	yInput = new QLineEdit(this);
	xInput->setPlaceholderText("X");
	yInput->setPlaceholderText("Y");
	xInput->setValidator(doubleValidator);
	yInput->setValidator(doubleValidator);
	xInputLayout->setContentsMargins(0, 0, 0, 0);
	yInputLayout->setContentsMargins(0, 0, 0, 0);

	xInputLayout->addWidget(xInput);
	yInputLayout->addWidget(yInput);
	layout->addLayout(xInputLayout);
	layout->addLayout(yInputLayout);
	layout->setContentsMargins(0, 0, 0, 0);

	// expose child signals
	QObject::connect(xInput, SIGNAL(textChanged(const QString&)), this, SLOT(xChanged(const QString&)));
	QObject::connect(yInput, SIGNAL(textChanged(const QString&)), this, SLOT(yChanged(const QString&)));
}

bool VectorInput::isIncomplete()
{
	if (xInput->text().length() == 0 || yInput->text().length() == 0) return true;
	else return false;
}

QString VectorInput::x() { return xInput->text(); }
QString VectorInput::y() { return yInput->text(); }

void VectorInput::clear() { xInput->clear(); yInput->clear(); }
void VectorInput::setXText(const QString& text) { xInput->setText(text); }
void VectorInput::setYText(const QString& text) { yInput->setText(text); }

// forwards signal from either lineedit
void VectorInput::xChanged(const QString& text) { emit textChanged(text, yInput->text()); }
void VectorInput::yChanged(const QString& text) { emit textChanged(xInput->text(), text); }

RGBInput::RGBInput(QWidget* parent) : QWidget(parent)
{
	rgbLayout = new QGridLayout(this);
	this->setLayout(rgbLayout);
	this->setFixedHeight(90);

	// background red control
	rSlider = new QSlider(Qt::Orientation::Horizontal, this);
	rSlider->setTickPosition(QSlider::NoTicks);
	rSlider->setRange(0, 255);
	rSlider->setSliderPosition(255);
	rSlider->setMinimumWidth(100);
	rSlider->setObjectName("rSlider");
	rSpinBox = new QSpinBox(this);
	rSpinBox->setRange(0, 255);
	rSpinBox->setValue(255);
	
	rSlider->setStyleSheet(slider);
	rSpinBox->setStyleSheet(spinbox);

	// background green control
	gSlider = new QSlider(Qt::Orientation::Horizontal, this);
	gSlider->setTickPosition(QSlider::NoTicks);
	gSlider->setRange(0, 255);
	gSlider->setSliderPosition(255);
	gSlider->setObjectName("gSlider");
	gSpinBox = new QSpinBox(this);
	gSpinBox->setRange(0, 255);
	gSpinBox->setValue(255);

	gSlider->setStyleSheet(slider);
	gSpinBox->setStyleSheet(spinbox);

	// background blue control
	bSlider = new QSlider(Qt::Orientation::Horizontal, this);
	bSlider->setTickPosition(QSlider::NoTicks);
	bSlider->setRange(0, 255);
	bSlider->setSliderPosition(255);
	bSlider->setObjectName("bSlider");
	bSpinBox = new QSpinBox(this);
	bSpinBox->setRange(0, 255);
	bSpinBox->setValue(255);

	bSlider->setStyleSheet(slider);
	bSpinBox->setStyleSheet(spinbox);

	// couple sliders and spinboxes, and expose spinbox signals
	QObject::connect(rSlider,  SIGNAL(valueChanged(int)), rSpinBox, SLOT(setValue(int)));
	QObject::connect(gSlider,  SIGNAL(valueChanged(int)), gSpinBox, SLOT(setValue(int)));
	QObject::connect(bSlider,  SIGNAL(valueChanged(int)), bSpinBox, SLOT(setValue(int)));
	QObject::connect(rSpinBox, SIGNAL(valueChanged(int)), rSlider,  SLOT(setValue(int)));
	QObject::connect(gSpinBox, SIGNAL(valueChanged(int)), gSlider,  SLOT(setValue(int)));
	QObject::connect(bSpinBox, SIGNAL(valueChanged(int)), bSlider,  SLOT(setValue(int)));
	QObject::connect(rSpinBox, SIGNAL(valueChanged(int)), this,   SIGNAL(rValueChanged(int)));
	QObject::connect(gSpinBox, SIGNAL(valueChanged(int)), this,   SIGNAL(gValueChanged(int)));
	QObject::connect(bSpinBox, SIGNAL(valueChanged(int)), this,   SIGNAL(bValueChanged(int)));

	// add sliders to layout
	rgbLayout->addWidget(rSlider, 0, 0); rgbLayout->addWidget(rSpinBox, 0, 1);
	rgbLayout->addWidget(gSlider, 1, 0); rgbLayout->addWidget(gSpinBox, 1, 1);
	rgbLayout->addWidget(bSlider, 2, 0); rgbLayout->addWidget(bSpinBox, 2, 1);
	rgbLayout->setContentsMargins(0, 0, 0, 0);
}

void RGBInput::setValues(int r, int g, int b)
{
	rSpinBox->setValue(r);
	gSpinBox->setValue(g);
	bSpinBox->setValue(b);
}