#include "../include/ControlPanel.h"

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
	
	// set size of window
	setFixedSize(300, 500);

	// create and position button
	button = new QPushButton("Hello World", this);
	button->setGeometry(10, 10, 80, 30);
}