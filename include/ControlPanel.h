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
#include <QtWidgets/qcheckbox.h>

class ControlPanel : public QMainWindow
{
	Q_OBJECT

private:
	Solver* solver;

	QString separationLineStyle = QString("background-color: #c0c0c0;");
	QString invalid = QString("background-color: #ffb0b0");
	QString valid = QString("background-color: white");
	QString main = QString(
		"QGroupBox { \
			border-radius: 5px; \
			border: 1px solid #d8d8d8; \
			margin: 8px 0px; \
			padding: 8px; \
		} \
		QGroupBox::title { \
			subcontrol-origin: margin; \
			subcontrol-position: top left; \
			left: 10px;	\
		}");
	QString danger = QString(
		"QPushButton { \
			border: 1px solid #d8d8d8; \
			background-color: white; \
			border-radius: 4px; \
			padding: 2px 20px; \
			color: #b00000; \
			font-weight: bold; \
		} \
		QPushButton:hover { \
			border: 1px solid #770000; \
			border-radius: 4px; \
			background-color: #b00000; \
			padding: 2px 20px; \
			color: white; \
			font-weight: bold; \
		}");

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
	// restart dialog buttons
	QPushButton* yes;
	QPushButton* no;

	// parameter inputs
	QComboBox* fpsDropdown;
	QLineEdit* substepsInput;
	QLineEdit* maxObjectsInput;
	VectorInput* gInput;

	QLabel* paramStatus;

	QPushButton* paramApplyButton;

	// spawner form inputs
	QLineEdit* idInput;
	VectorInput* posInput;
	VectorInput* velInput;
	QLineEdit* intervalInput;
	QCheckBox* active;
	QCheckBox* visible;
	QPushButton* addButton;
	QPushButton* clearButton;

	// TEMP: spawner edit
	QPushButton* editButton;

	// spawning widgets
	QListWidget* spawnerList;

private slots:
	void togglePauseButton();
	void restartDialogHandler();
	void updateParam();
	void addSpawner();

public slots:
	void receiveSpawner(Spawner*);
	void receiveSpawnerIDs(std::vector<std::string>);

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
	void addSpawner(SpawnerDTO);
	void getSpawner(std::string);
	void getSpawnerIDs();
};

#endif