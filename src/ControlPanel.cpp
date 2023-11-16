#include "../include/ControlPanel.h"

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

	globalLayout = new QVBoxLayout();
	globalLayout->addLayout(taskbarLayout);
	globalLayout->addLayout(bgBallLayout);
	globalLayout->addWidget(spawningGroup);

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
	// background red control
	QLabel* bgRLabel = new QLabel("Red", this);
	bgRLabel->setAlignment(Qt::AlignRight);
	QSlider* bgRSlider = new QSlider(Qt::Orientation::Horizontal, this);
	bgRSlider->setTickPosition(QSlider::NoTicks);
	bgRSlider->setRange(0, 255);
	bgRSlider->setSliderPosition(255);
	bgRSlider->setMinimumWidth(100);
	QSpinBox* bgRSpin = new QSpinBox(this);
	bgRSpin->setRange(0, 255);
	bgRSpin->setValue(255);

	// background green control
	QLabel* bgGLabel = new QLabel("Green", this);
	bgGLabel->setAlignment(Qt::AlignRight);
	QSlider* bgGSlider = new QSlider(Qt::Orientation::Horizontal, this);
	bgGSlider->setTickPosition(QSlider::NoTicks);
	bgGSlider->setRange(0, 255);
	bgGSlider->setSliderPosition(255);
	QSpinBox* bgGSpin = new QSpinBox(this);
	bgGSpin->setRange(0, 255);
	bgGSpin->setValue(255);

	// background blue control
	QLabel* bgBLabel = new QLabel("Blue", this);
	bgBLabel->setAlignment(Qt::AlignRight);
	QSlider* bgBSlider = new QSlider(Qt::Orientation::Horizontal, this);
	bgBSlider->setTickPosition(QSlider::NoTicks);
	bgBSlider->setRange(0, 255);
	bgBSlider->setSliderPosition(255);
	QSpinBox* bgBSpin = new QSpinBox(this);
	bgBSpin->setRange(0, 255);
	bgBSpin->setValue(255);

	// couple sliders and spinboxes, and connect spinbox to renderer
	QObject::connect(bgRSlider, SIGNAL(valueChanged(int)), bgRSpin,   SLOT(setValue(int)));
	QObject::connect(bgGSlider, SIGNAL(valueChanged(int)), bgGSpin,   SLOT(setValue(int)));
	QObject::connect(bgBSlider, SIGNAL(valueChanged(int)), bgBSpin,   SLOT(setValue(int)));
	QObject::connect(bgRSpin,   SIGNAL(valueChanged(int)), bgRSlider, SLOT(setValue(int)));
	QObject::connect(bgGSpin,   SIGNAL(valueChanged(int)), bgGSlider, SLOT(setValue(int)));
	QObject::connect(bgBSpin,   SIGNAL(valueChanged(int)), bgBSlider, SLOT(setValue(int)));
	QObject::connect(bgRSpin,   SIGNAL(valueChanged(int)), renderer,  SLOT(setBackgroundRed(int)));
	QObject::connect(bgGSpin,   SIGNAL(valueChanged(int)), renderer,  SLOT(setBackgroundGreen(int)));
	QObject::connect(bgBSpin,   SIGNAL(valueChanged(int)), renderer,  SLOT(setBackgroundBlue(int)));

	// add sliders to layout
	bgColourLayout = new QGridLayout(this);
	bgColourLayout->addWidget(bgRLabel, 0, 0); bgColourLayout->addWidget(bgRSlider, 0, 1); bgColourLayout->addWidget(bgRSpin, 0, 2);
	bgColourLayout->addWidget(bgGLabel, 1, 0); bgColourLayout->addWidget(bgGSlider, 1, 1); bgColourLayout->addWidget(bgGSpin, 1, 2);
	bgColourLayout->addWidget(bgBLabel, 2, 0); bgColourLayout->addWidget(bgBSlider, 2, 1); bgColourLayout->addWidget(bgBSpin, 2, 2);
}

void ControlPanel::initBallColour(Renderer* renderer)
{
	QWidget* sliderGroup		  = new QWidget(this);
	QGridLayout* ballSliderLayout = new QGridLayout();
	sliderGroup->setLayout(ballSliderLayout);
	
	// separate ball colour radio buttons
	QButtonGroup* ballColourGroup = new QButtonGroup(this);
	QRadioButton* randomRadio	  = new QRadioButton(this);
	QRadioButton* rgbRadio		  = new QRadioButton(this);
	ballColourGroup->addButton(randomRadio, 0);
	ballColourGroup->addButton(rgbRadio, 1);

	// ball red control
	QLabel* ballRLabel = new QLabel("Red", this);
	ballRLabel->setAlignment(Qt::AlignRight);
	QSlider* ballRSlider = new QSlider(Qt::Orientation::Horizontal, sliderGroup);
	ballRSlider->setTickPosition(QSlider::NoTicks);
	ballRSlider->setRange(0, 255);
	ballRSlider->setSliderPosition(255);
	ballRSlider->setMinimumWidth(100);
	QSpinBox* ballRSpin = new QSpinBox(sliderGroup);
	ballRSpin->setRange(0, 255);
	ballRSpin->setValue(255);

	// ball green control
	QLabel* ballGLabel = new QLabel("Green", this);
	ballGLabel->setAlignment(Qt::AlignRight);
	QSlider* ballGSlider = new QSlider(Qt::Orientation::Horizontal, sliderGroup);
	ballGSlider->setTickPosition(QSlider::NoTicks);
	ballGSlider->setRange(0, 255);
	ballGSlider->setSliderPosition(255);
	QSpinBox* ballGSpin = new QSpinBox(sliderGroup);
	ballGSpin->setRange(0, 255);
	ballGSpin->setValue(255);

	// ball blue control
	QLabel* ballBLabel = new QLabel("Blue", this);
	ballBLabel->setAlignment(Qt::AlignRight);
	QSlider* ballBSlider = new QSlider(Qt::Orientation::Horizontal, sliderGroup);
	ballBSlider->setTickPosition(QSlider::NoTicks);
	ballBSlider->setRange(0, 255);
	ballBSlider->setSliderPosition(255);
	QSpinBox* ballBSpin = new QSpinBox(sliderGroup);
	ballBSpin->setRange(0, 255);
	ballBSpin->setValue(255);

	// add sliders to layout
	ballSliderLayout->addWidget(ballRLabel, 0, 0); ballSliderLayout->addWidget(ballRSlider, 0, 1); ballSliderLayout->addWidget(ballRSpin, 0, 2);
	ballSliderLayout->addWidget(ballGLabel, 1, 0); ballSliderLayout->addWidget(ballGSlider, 1, 1); ballSliderLayout->addWidget(ballGSpin, 1, 2);
	ballSliderLayout->addWidget(ballBLabel, 2, 0); ballSliderLayout->addWidget(ballBSlider, 2, 1); ballSliderLayout->addWidget(ballBSpin, 2, 2);

	ballColourLayout = new QGridLayout(this);
	ballColourLayout->addWidget(randomRadio, 0, 0);
	ballColourLayout->addWidget(new QLabel("Random"), 0, 1);
	ballColourLayout->addWidget(rgbRadio, 1, 0, Qt::AlignTop);
	ballColourLayout->addWidget(sliderGroup, 1, 1);

	// connect radio buttons to renderer
	QObject::connect(randomRadio, SIGNAL(toggled(bool)), renderer, SLOT(toggleRandom(bool)));
	QObject::connect(randomRadio, SIGNAL(toggled(bool)), sliderGroup, SLOT(setDisabled(bool)));
	//QObject::connect(rgbRadio, SIGNAL(toggled(bool)), ballRSlider, SLOT(setEnabled(bool)));
	// couple sliders and spinboxes, and connect spinboxes to renderer
	QObject::connect(ballRSlider, SIGNAL(valueChanged(int)), ballRSpin,   SLOT(setValue(int)));
	QObject::connect(ballGSlider, SIGNAL(valueChanged(int)), ballGSpin,   SLOT(setValue(int)));
	QObject::connect(ballBSlider, SIGNAL(valueChanged(int)), ballBSpin,   SLOT(setValue(int)));
	QObject::connect(ballRSpin,   SIGNAL(valueChanged(int)), ballRSlider, SLOT(setValue(int)));
	QObject::connect(ballGSpin,   SIGNAL(valueChanged(int)), ballGSlider, SLOT(setValue(int)));
	QObject::connect(ballBSpin,   SIGNAL(valueChanged(int)), ballBSlider, SLOT(setValue(int)));
	QObject::connect(ballRSpin,   SIGNAL(valueChanged(int)), renderer,    SLOT(setBallRed(int)));
	QObject::connect(ballGSpin,   SIGNAL(valueChanged(int)), renderer,    SLOT(setBallGreen(int)));
	QObject::connect(ballBSpin,   SIGNAL(valueChanged(int)), renderer,    SLOT(setBallBlue(int)));

	// default values
	ballRSpin->setValue(255);
	ballGSpin->setValue(0);
	ballBSpin->setValue(0);
	randomRadio->setChecked(true);
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
	QGridLayout* spawnerFormLayout = new QGridLayout(this);

	QLineEdit* idInput = new QLineEdit(this);
	idInput->setPlaceholderText("ex. spawner1");
	QRegularExpression idRe("^[a-zA-Z0-9_]+$");
	QRegularExpressionValidator* idValidator = new QRegularExpressionValidator(idRe, this);
	idInput->setValidator(idValidator);

	QDoubleValidator* doubleValidator = new QDoubleValidator(this);

	// spawner position input
	QWidget* posInput = new QWidget(this);
	QHBoxLayout* posInputLayout = new QHBoxLayout(posInput);
	posInput->setLayout(posInputLayout);
	posXInput = new QLineEdit(posInput);
	posYInput = new QLineEdit(posInput);
	posXInput->setPlaceholderText("200.0");
	posYInput->setPlaceholderText("400.0");
	posXInput->setValidator(doubleValidator);
	posYInput->setValidator(doubleValidator);

	posInputLayout->addWidget(new QLabel("X"));
	posInputLayout->addWidget(posXInput);
	posInputLayout->addWidget(new QLabel("Y"));
	posInputLayout->addWidget(posYInput);

	// spawner velocity input
	QWidget* velInput = new QWidget(this);
	QHBoxLayout* velInputLayout = new QHBoxLayout(velInput);
	velInput->setLayout(velInputLayout);
	velXInput = new QLineEdit(velInput);
	velYInput = new QLineEdit(velInput);
	velXInput->setPlaceholderText("1000.0");
	velYInput->setPlaceholderText("-500.0");
	velXInput->setValidator(doubleValidator);
	velYInput->setValidator(doubleValidator);

	velInputLayout->addWidget(new QLabel("X"));
	velInputLayout->addWidget(velXInput);
	velInputLayout->addWidget(new QLabel("Y"));
	velInputLayout->addWidget(velYInput);

	spawnerFormLayout->addWidget(new QLabel("ID:"),		  0, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(new QLabel("Position:"), 1, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(new QLabel("Velocity:"), 2, 0, Qt::AlignRight);
	spawnerFormLayout->addWidget(idInput,  0, 1);
	spawnerFormLayout->addWidget(posInput, 1, 1);
	spawnerFormLayout->addWidget(velInput, 2, 1);
	QPushButton* addButton = new QPushButton("Add", this);

	spawnerList = new QListWidget(this);

	// add to layout
	QHBoxLayout* spawningOptionsLayout = new QHBoxLayout(this);
	spawningOptionsLayout->addStretch();
	spawningOptionsLayout->addWidget(autoRadio);
	spawningOptionsLayout->addWidget(freeRadio);
	spawningOptionsLayout->addStretch();
	

	spawningLayout = new QVBoxLayout(this);
	spawningLayout->addLayout(spawningOptionsLayout);
	spawningLayout->addLayout(spawnerFormLayout);
	spawningLayout->addWidget(addButton, 0, Qt::AlignRight);
	spawningLayout->addWidget(spawnerList);

	// connect spawner radio buttons to solver
	QObject::connect(autoRadio, SIGNAL(toggled(bool)), solver, SLOT(setAutoSpawning(bool)));

	// default values
	autoRadio->setChecked(true);
}