#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "../include/Solver.h"
#include "../include/Renderer.h"
#include "../include/CustomWidgets.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qcombobox.h>

class ControlPanel : public QMainWindow
{
	Q_OBJECT

private:
	QString separationLineStyle = QString("background-color: #c0c0c0;");
	QString invalid = QString("background-color: #ffb0b0");
	QString valid = QString("background-color: white");

	QVBoxLayout* globalLayout;
	QHBoxLayout* bgBallLayout;
	QHBoxLayout* paramSpawnLayout;

	QHBoxLayout* taskbarLayout;
	QHBoxLayout* bgColourLayout;
	QGridLayout* ballColourLayout;
	QVBoxLayout* parameterLayout;
	QVBoxLayout* spawningLayout;

	// taskbar widgets
	QPushButton* pauseButton;
	QPushButton* restartButton;
	QMessageBox* restartDialog;

	// parameter inputs
	QComboBox* fpsDropdown;
	QLineEdit* substepsInput;
	QLineEdit* maxObjectsInput;
	VectorInput* gInput;

	QLabel* paramStatus;

	QPushButton* paramApplyButton;

	// spawner form inputs
	VectorInput* posInput;
	VectorInput* velInput;
	QPushButton* addButton;
	QPushButton* clearButton;

	// spawning widgets
	QListWidget* spawnerList;

private slots:
	void togglePauseButton();
	void restartDialogHandler();
	void updateParamDisplay();

public:
	ControlPanel(Solver* solver = 0, Renderer* renderer = 0);

	void initTaskbar(Solver* solver);
	void initBgColour(Renderer* renderer);
	void initBallColour(Renderer* renderer);
	void initParameter(Solver* solver);
	void initSpawning(Solver* solver);

signals:
	void restartConfirmed();
	void applyFramerate(int);
	void applySubsteps(int);
	void applyMaxObjects(int);
	void applyGravity(float, float);
};

#endif