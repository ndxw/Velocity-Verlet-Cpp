#include "../include/CustomWidgets.h"

#include <QtWidgets/qlabel.h>

VectorInput::VectorInput(Orientation orient)
{
	if (orient == Orientation::Horizontal) { layout = new QHBoxLayout(this); }
	else { layout = new QVBoxLayout(this); }

	this->setLayout(layout);

	QDoubleValidator* doubleValidator = new QDoubleValidator(this);

	QHBoxLayout* xInputLayout = new QHBoxLayout(this);
	QHBoxLayout* yInputLayout = new QHBoxLayout(this);
	xInput = new QLineEdit(this);
	yInput = new QLineEdit(this);
	xInput->setPlaceholderText("50.0");
	yInput->setPlaceholderText("100.0");
	xInput->setValidator(doubleValidator);
	yInput->setValidator(doubleValidator);
	xInputLayout->setContentsMargins(0, 0, 0, 0);
	yInputLayout->setContentsMargins(0, 0, 0, 0);

	xInputLayout->addWidget(new QLabel("\tX"), 0, Qt::AlignRight);
	xInputLayout->addWidget(xInput);
	yInputLayout->addWidget(new QLabel("\tY"), 0, Qt::AlignRight);
	yInputLayout->addWidget(yInput);
	layout->addLayout(xInputLayout);
	layout->addLayout(yInputLayout);
	layout->setContentsMargins(0, 0, 0, 0);

	// TODO: expose child signals and slots
}

RGBInput::RGBInput()
{

}