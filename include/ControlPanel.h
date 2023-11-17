#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "../include/Solver.h"
#include "../include/Renderer.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlistwidget.h>

class ControlPanel : public QMainWindow
{
	Q_OBJECT

private:
	QString separationLineStyle = QString("background-color: #c0c0c0;");

	QVBoxLayout* globalLayout;
	QHBoxLayout* bgBallLayout;
	QHBoxLayout* paramSpawnLayout;

	QHBoxLayout* taskbarLayout;
	QGridLayout* bgColourLayout;
	QGridLayout* ballColourLayout;
	QGridLayout* parameterLayout;
	QVBoxLayout* spawningLayout;

	// taskbar widgets
	QPushButton* pauseButton;
	QPushButton* restartButton;
	QMessageBox* restartDialog;

	// parameter inputs
	//QComboBox* fpsDropdown;
	QLineEdit* substepsInput;
	QLineEdit* maxObjectsInput;
	QLineEdit* gXInput;
	QLineEdit* gYInput;
	// current parameter values
	QLabel* fps;
	QLabel* substeps;
	QLabel* maxObjects;
	QLabel* gravity;

	// spawner form inputs
	QLineEdit* posXInput;
	QLineEdit* posYInput;
	QLineEdit* velXInput;
	QLineEdit* velYInput;

	// spawning widgets
	QListWidget* spawnerList;

private slots:
	void togglePauseButton();
	void restartDialogHandler();

public:
	ControlPanel(Solver* solver = 0, Renderer* renderer = 0);

	void initTaskbar(Solver* solver);
	void initBgColour(Renderer* renderer);
	void initBallColour(Renderer* renderer);
	void initParameter(Solver* solver);
	void initSpawning(Solver* solver);

signals:
	void restartConfirmed();

};

#endif