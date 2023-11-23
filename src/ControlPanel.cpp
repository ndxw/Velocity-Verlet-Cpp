#include "../include/ControlPanel.h"

#include <string>

#include <QtWidgets/qslider.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qgroupbox.h>

#include <QtCore/qregularexpression.h>

#include <QtGui/qvalidator.h>


ControlPanel::ControlPanel(Solver* solver, Renderer* renderer) 
{

	// window parameters===============================================
	setWindowTitle("Control Panel");
	resize(500, 500);

	QWidget* centralWid = new QWidget(this);
	setCentralWidget(centralWid);
	
	// init sections===================================================
	initTaskbar(solver);

	initBgColour(renderer);
	QGroupBox* bgColourGroup = new QGroupBox("Background Colour");
	bgColourGroup->setLayout(bgColourLayout);

	initBallColour(renderer);
	QGroupBox* ballColourGroup = new QGroupBox("Ball Colour");
	ballColourGroup->setLayout(ballColourLayout);

	initParameter(solver);
	QGroupBox* paramGroup = new QGroupBox("Simulation Parameters");
	paramGroup->setLayout(parameterLayout);

	initSpawning(solver);
	QGroupBox* spawningGroup = new QGroupBox("Spawners");
	spawningGroup->setLayout(spawningLayout);

	// init dialogs====================================================
	restartDialog = new QMessageBox(this);
	restartDialog->setText("Are you sure you want to restart the simulation?");
	restartDialog->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	restartDialog->setDefaultButton(QMessageBox::No);
	restartDialog->setIcon(QMessageBox::Icon::Warning);
	
	// add to global layout============================================
	bgBallLayout = new QHBoxLayout();
	bgBallLayout->addWidget(bgColourGroup);
	bgBallLayout->addWidget(ballColourGroup);

	paramSpawnLayout = new QHBoxLayout();
	paramSpawnLayout->addWidget(paramGroup);
	paramSpawnLayout->addWidget(spawningGroup);

	globalLayout = new QVBoxLayout();
	globalLayout->addLayout(taskbarLayout);
	globalLayout->addLayout(bgBallLayout);
	globalLayout->addLayout(paramSpawnLayout);

	centralWid->setLayout(globalLayout);
}

void ControlPanel::initTaskbar(Solver* solver)
{
	pauseButton	  = new QPushButton("Pause",   this);
	restartButton = new QPushButton("Restart", this);

	// add buttons to taskbar layout
	taskbarLayout = new QHBoxLayout(this);
	taskbarLayout->addWidget(pauseButton);
	taskbarLayout->addWidget(restartButton);

	// connect signals to solver
	QObject::connect(pauseButton, SIGNAL(clicked(bool)), solver, SLOT(togglePause()));			// pause solver
	QObject::connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(togglePauseButton()));		// toggle pause button text
	QObject::connect(restartButton, SIGNAL(clicked(bool)), this, SLOT(restartDialogHandler()));	// open restart dialog
	QObject::connect(this, SIGNAL(restartConfirmed()), solver, SLOT(restart()));				// relay restart dialog result to solver
}

void ControlPanel::togglePauseButton() 
{
	if (pauseButton->text().contains(QString("Pause"))) { pauseButton->setText(QString("Play")); }
	else { pauseButton->setText(QString("Pause")); }
}

void ControlPanel::restartDialogHandler()
{
	int result = restartDialog->exec();
	if (result == QMessageBox::Yes) emit restartConfirmed();
}

void ControlPanel::initBgColour(Renderer* renderer)
{
	bgColourLayout = new QHBoxLayout(this);

	RGBInput* bgRGB = new RGBInput(this);

	QObject::connect(bgRGB, SIGNAL(rValueChanged(int)), renderer, SLOT(setBackgroundRed(int)));
	QObject::connect(bgRGB, SIGNAL(gValueChanged(int)), renderer, SLOT(setBackgroundGreen(int)));
	QObject::connect(bgRGB, SIGNAL(bValueChanged(int)), renderer, SLOT(setBackgroundBlue(int)));

	bgColourLayout->addWidget(bgRGB);
}

void ControlPanel::initBallColour(Renderer* renderer)
{
	
	// separate ball colour radio buttons
	QButtonGroup* ballColourGroup = new QButtonGroup(this);
	QRadioButton* randomRadio	  = new QRadioButton(this);
	QRadioButton* rgbRadio		  = new QRadioButton(this);
	ballColourGroup->addButton(randomRadio, 0);
	ballColourGroup->addButton(rgbRadio, 1);

	RGBInput* ballRGB = new RGBInput(this);

	ballColourLayout = new QGridLayout(this);
	ballColourLayout->addWidget(randomRadio, 0, 0);
	ballColourLayout->addWidget(new QLabel("Random"), 0, 1);
	ballColourLayout->addWidget(rgbRadio, 1, 0, Qt::AlignTop);
	ballColourLayout->addWidget(ballRGB, 1, 1);

	// connect radio buttons to renderer
	QObject::connect(randomRadio, SIGNAL(toggled(bool)), renderer, SLOT(toggleRandom(bool)));
	QObject::connect(randomRadio, SIGNAL(toggled(bool)), ballRGB, SLOT(setDisabled(bool)));

	// couple sliders and spinboxes, and connect spinboxes to renderer
	QObject::connect(ballRGB, SIGNAL(rValueChanged(int)), renderer, SLOT(setBallRed(int)));
	QObject::connect(ballRGB, SIGNAL(gValueChanged(int)), renderer, SLOT(setBallGreen(int)));
	QObject::connect(ballRGB, SIGNAL(bValueChanged(int)), renderer, SLOT(setBallBlue(int)));

	// default values
	ballRGB->setValues(255, 0, 0);
	randomRadio->setChecked(true);
}

void ControlPanel::initParameter(Solver* solver)
{
	// framerate
	QLabel* fps = new QLabel("Framerate", this);
	fps->setAlignment(Qt::AlignRight);
	fpsDropdown = new QComboBox(this);
	fpsDropdown->addItem("12");
	fpsDropdown->addItem("24");
	fpsDropdown->addItem("30");
	fpsDropdown->addItem("48");
	fpsDropdown->addItem("60");
	fpsDropdown->addItem("120");
	fpsDropdown->addItem("144");
	fpsDropdown->addItem("240");
	fpsDropdown->setDisabled(true);

	// substeps
	QLabel* substeps = new QLabel("Substeps", this);
	substeps->setAlignment(Qt::AlignRight);
	substepsInput = new QLineEdit(this);
	substepsInput->setValidator(new QIntValidator(1, 16, this));
	substepsInput->setPlaceholderText("1-16");
	substepsInput->setText(QString::fromStdString(std::to_string(solver->getSubsteps())));

	// max objects
	QLabel* maxObjects = new QLabel("Max. Objects", this);
	maxObjects->setAlignment(Qt::AlignRight);
	maxObjectsInput = new QLineEdit(this);
	maxObjectsInput->setValidator(new QIntValidator(0, 999'999, this));
	maxObjectsInput->setPlaceholderText("ex. 1000");
	maxObjectsInput->setText(QString::fromStdString(std::to_string(solver->getMaxObjects())));

	// gravity
	QLabel* g = new QLabel("Gravity", this);
	g->setAlignment(Qt::AlignRight);
	gInput = new VectorInput(VectorInput::Orientation::Horizontal, this);
	Vec2D gravity = solver->getGravity();
	gInput->setXText(QString::fromStdString(std::to_string(gravity.x())));
	gInput->setYText(QString::fromStdString(std::to_string(-gravity.y())));

	// status message
	paramStatus = new QLabel("Parameters applied!", this);
	paramStatus->setStyleSheet("font-style:italic");
	paramStatus->setVisible(false);

	// apply button
	QHBoxLayout* paramApplyLayout = new QHBoxLayout(this);
	paramApplyButton = new QPushButton("Apply", this);
	paramApplyLayout->addStretch();
	paramApplyLayout->addWidget(paramApplyButton);

	QGridLayout* paramInputLayout = new QGridLayout(this);
	paramInputLayout->addWidget(fps, 0, 0);
	paramInputLayout->addWidget(substeps, 1, 0);
	paramInputLayout->addWidget(maxObjects, 2, 0);
	paramInputLayout->addWidget(g, 3, 0);
	paramInputLayout->addWidget(fpsDropdown, 0, 1);
	paramInputLayout->addWidget(substepsInput, 1, 1);
	paramInputLayout->addWidget(maxObjectsInput, 2, 1);
	paramInputLayout->addWidget(gInput, 3, 1);

	parameterLayout = new QVBoxLayout(this);
	parameterLayout->addLayout(paramInputLayout);
	parameterLayout->addWidget(paramStatus);
	parameterLayout->addStretch();
	parameterLayout->addLayout(paramApplyLayout);

	// button sends parameters to solver
	QObject::connect(paramApplyButton, SIGNAL(clicked(bool)), this, SLOT(updateParamDisplay()));
	QObject::connect(this, SIGNAL(applyFramerate(int)),    solver, SLOT(setFramerate(int)));
	QObject::connect(this, SIGNAL(applySubsteps(int)),     solver, SLOT(setSubsteps(int)));
	QObject::connect(this, SIGNAL(applyMaxObjects(int)),   solver, SLOT(setMaxObjects(int)));
	QObject::connect(this, SIGNAL(applyGravity(float, float)), solver, SLOT(setGravity(float, float)));
	// stylesheets for lineedits
	QObject::connect(substepsInput, &QLineEdit::textChanged, this, [=]() { substepsInput->setStyleSheet(valid); paramStatus->setVisible(false); });
	QObject::connect(maxObjectsInput, &QLineEdit::textChanged, this, [=]() { maxObjectsInput->setStyleSheet(valid); paramStatus->setVisible(false); });
	QObject::connect(gInput, &VectorInput::textChanged, this, [=]() { gInput->setStyleSheet(valid); paramStatus->setVisible(false); });

	// default values
	fpsDropdown->setCurrentIndex(4);	// 60 fps
	
}

void ControlPanel::updateParamDisplay()
{
	// at least one line edit is empty
	if (substepsInput->text().length() == 0 ||
		maxObjectsInput->text().length() == 0 ||
		gInput->isIncomplete())
	{
		// highlight invalid lineedit
		if (substepsInput->text().length() == 0) {
			substepsInput->setStyleSheet(invalid);
		}
		if (maxObjectsInput->text().length() == 0) {
			maxObjectsInput->setStyleSheet(invalid);
		}
		if (gInput->isIncomplete()) {
			gInput->setStyleSheet(invalid);
		}

		return;
	}

	// send values to solver
	emit applyFramerate(std::stoi(fpsDropdown->currentText().toStdString()));
	emit applySubsteps(std::stoi(substepsInput->text().toStdString()));
	emit applyMaxObjects(std::stoi(maxObjectsInput->text().toStdString()));
	emit applyGravity(std::stof(gInput->x().toStdString()), std::stof(gInput->y().toStdString()) * -1.f);	// flip y because window origin is top left
	paramStatus->setVisible(true);
}

void ControlPanel::initSpawning(Solver* solver)
{
	// separate spawning options buttons
	QButtonGroup* spawningOptionsGroup = new QButtonGroup(this);
	QRadioButton* autoRadio = new QRadioButton("Auto", this);
	QRadioButton* freeRadio = new QRadioButton("Free", this);
	spawningOptionsGroup->addButton(autoRadio, 0);
	spawningOptionsGroup->addButton(freeRadio, 1);

	// spawner creation form
	QLineEdit* idInput = new QLineEdit(this);
	idInput->setPlaceholderText("ex. spawner1");
	QRegularExpression idRe("^[a-zA-Z0-9_-]+$");
	QRegularExpressionValidator* idValidator = new QRegularExpressionValidator(idRe, this);
	idInput->setValidator(idValidator);

	// position and velocity input
	posInput = new VectorInput(VectorInput::Orientation::Horizontal, this);
	velInput = new VectorInput(VectorInput::Orientation::Horizontal, this);

	// buttons
	addButton = new QPushButton("Add", this);
	clearButton = new QPushButton("Clear", this);
	QHBoxLayout* spawnerFormButtonLayout = new QHBoxLayout(this);
	spawnerFormButtonLayout->addStretch();
	spawnerFormButtonLayout->addWidget(clearButton);
	spawnerFormButtonLayout->addWidget(addButton);
	spawnerFormButtonLayout->setContentsMargins(0, 0, 0, 0);

	QGridLayout* spawnerFormLayout = new QGridLayout(this);
	spawnerFormLayout->addWidget(new QLabel("ID:"),		  0, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(new QLabel("Position:"), 1, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(new QLabel("Velocity:"), 2, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(idInput, 0, 1);
	spawnerFormLayout->addWidget(posInput, 1, 1);
	spawnerFormLayout->addWidget(velInput, 2, 1);
	spawnerFormLayout->addLayout(spawnerFormButtonLayout, 3, 1);
	QGroupBox* spawnerFormGroup = new QGroupBox("Create Spawner");
	spawnerFormGroup->setLayout(spawnerFormLayout);

	spawnerList = new QListWidget(this);

	// add to layout
	QVBoxLayout* spawningOptionsLayout = new QVBoxLayout(this);
	spawningOptionsLayout->addWidget(autoRadio);
	spawningOptionsLayout->addWidget(freeRadio);
	spawningOptionsLayout->addStretch();

	QHBoxLayout* spawnerOptionsFormLayout = new QHBoxLayout(this);
	spawnerOptionsFormLayout->addLayout(spawningOptionsLayout);
	spawnerOptionsFormLayout->addWidget(spawnerFormGroup);
	

	spawningLayout = new QVBoxLayout(this);
	spawningLayout->addLayout(spawnerOptionsFormLayout);
	spawningLayout->addWidget(spawnerList);

	// connect spawner radio buttons to solver
	QObject::connect(autoRadio, SIGNAL(toggled(bool)), solver, SLOT(setAutoSpawning(bool)));

	// default values
	autoRadio->setChecked(true);
}