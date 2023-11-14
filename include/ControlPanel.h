#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>

class ControlPanel : public QWidget
{
private:
	QPushButton* button;
public:
	ControlPanel(QWidget* parent = 0);



};

#endif