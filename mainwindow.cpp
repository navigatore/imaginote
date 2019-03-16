#include "mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <fstream>
#include "sonarspaceplayer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      keyLeftPressed(false),
      keyRightPressed(false),
      keyDownPressed(false),
      space(updateFreq),
      spaceLoaded(false) {
  ui->setupUi(this);
  ui->spaceLabel->setText("No space definition loaded");
  ui->presIntervalLineEdit->setText("1");
  playing = false;
  presIntValid = true;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000 / updateFreq);

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

  space.setMapWidget(ui->mapWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::update() {
  if (playing) {
    if (keyLeftPressed) {
      space.rotateListenerLeft(1.0f / updateFreq);
    } else if (keyRightPressed) {
      space.rotateListenerRight(1.0f / updateFreq);
    }
    if (keyUpPressed) {
      space.goForward(1.0f / updateFreq);
    } else if (keyDownPressed) {
      space.goBackward(1.0f / updateFreq);
    }
    space.update(1.0f / updateFreq);

    updateListenerPos();
    updateListenerAngle();
  }
}

void MainWindow::keyPressEvent(QKeyEvent *ev) {
  if (ev->key() == Qt::Key_A) {
    keyLeftPressed = true;
  } else if (ev->key() == Qt::Key_D) {
    keyRightPressed = true;
  } else if (ev->key() == Qt::Key_W) {
    keyUpPressed = true;
  } else if (ev->key() == Qt::Key_S) {
    keyDownPressed = true;
  } else if (ev->key() == Qt::Key_F) {
    space.toggleMovingFocusAngle();
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev) {
  if (ev->key() == Qt::Key_A) {
    keyLeftPressed = false;
  } else if (ev->key() == Qt::Key_D) {
    keyRightPressed = false;
  } else if (ev->key() == Qt::Key_W) {
    keyUpPressed = false;
  } else if (ev->key() == Qt::Key_S) {
    keyDownPressed = false;
  }
}

void MainWindow::loadSpaceDef() {
  ui->startStopButton_1->setEnabled(false);
  ui->startStopButton_2->setEnabled(false);
  spaceLoaded = false;

  QString fileName =
      QFileDialog::getOpenFileName(this, "Select space definition file", "",
                                   "Space definition (*.spc);;All Files (*)");

  QByteArray ba = fileName.toLatin1();
  const char *c_str = ba.data();

  try {
    space.mapWidget = ui->mapWidget;
    space.loadFromFile(c_str);
    ui->spaceLabel->setText(space.getName().c_str());
    spaceLoaded = true;
    tryEnableStartStop();
  } catch (InvalidSpaceFile &) {
    ui->spaceLabel->setText("Invalid file!");
  }
}

void MainWindow::startStopClicked() {
  if (!playing) {
    // auto params = PresentationParams();
    //        params.headAngle = ui->horizontalSlider->value();
    //        params.presInterval = ui->presIntervalLineEdit->text().toDouble();
    //        if (ui->waveTypeCombo->currentText() == "Sinusoidal")
    //            params.waveType = PresentationParams::WaveType::SINUS;
    //        else if (ui->waveTypeCombo->currentText() == "Triangular")
    //            params.waveType = PresentationParams::WaveType::TRIANGLE;

    //        if (ui->soundPitchCombo->currentText() == "Distance")
    //            params.pitchUse = PresentationParams::PitchUse::DISTANCE;

    //        if (ui->soundTimbreCombo->currentText() == "Width / height ratio")
    //            params.timbreUse = PresentationParams::TimbreUse::WH_RATIO;

    // sp = new SpacePlayer(spaceObjects, params);
    ui->startStopButton->setText("Stop");
    ui->presIntervalLineEdit->setEnabled(false);
    ui->waveTypeCombo->setEnabled(false);
    ui->soundTimbreCombo->setEnabled(false);
    ui->soundPitchCombo->setEnabled(false);
    ui->loadSpaceButton->setEnabled(false);
    playing = true;
    ui->startStopButton->clearFocus();
  } else {
    // delete sp;
    ui->startStopButton->setText("Start");
    ui->presIntervalLineEdit->setEnabled(true);
    ui->waveTypeCombo->setEnabled(true);
    ui->soundTimbreCombo->setEnabled(true);
    ui->soundPitchCombo->setEnabled(true);
    ui->loadSpaceButton->setEnabled(true);
    playing = false;
  }
}

void MainWindow::startStopOneClicked() {
  if (!playing) {
    ui->startStopButton_1->setText("Stop");
    startClicked(new NewSpacePlayer());
  } else {
    ui->startStopButton_1->setText("Start");
    stopClicked();
  }
}

void MainWindow::startClicked(GenericSpacePlayer *sp) {
  ui->visualAngleSlider->setEnabled(false);
  ui->distanceLimitSlider->setEnabled(false);
  playing = true;
  ui->startStopButton->clearFocus();
  ui->loadSpaceButton->setEnabled(false);
  auto angleX = Angle(ui->visualAngleSlider->value());
  auto maxDistance = ui->distanceLimitSlider->value();
  space.startPlaying(angleX, maxDistance, sp);
}

void MainWindow::stopClicked() {
  ui->visualAngleSlider->setEnabled(true);
  ui->distanceLimitSlider->setEnabled(true);
  ui->loadSpaceButton->setEnabled(true);
  playing = false;
  space.stopPlaying();
  adjustSize();
}

void MainWindow::startSonarClicked() {
  if (!playing) {
    ui->startStopButton_2->setText("Stop");
    startClicked(new SonarSpacePlayer());
  } else {
    ui->startStopButton_2->setText("Start");
    stopClicked();
  }
}

void MainWindow::validatePresInterval(const QString &text) {
  if (is_number(text.toStdString()) && std::stod(text.toStdString()) > 0.1 &&
      std::stod(text.toStdString()) < 10) {
    presIntValid = true;
    ui->presIntervalLineEdit->setStyleSheet("");
  } else {
    presIntValid = false;
    ui->presIntervalLineEdit->setStyleSheet("QLineEdit { color: red; }");
    ui->startStopButton->setEnabled(false);
  }
  tryEnableStartStop();
}

bool MainWindow::is_number(const std::string &s) {
  std::size_t pos;
  try {
    std::stod(s, &pos);
  } catch (...) {
    return false;
  }
  return (pos == s.size());
}

void MainWindow::tryEnableStartStop() {
  if (spaceLoaded && presIntValid) ui->startStopButton->setEnabled(true);
  if (spaceLoaded) ui->startStopButton_1->setEnabled(true);
  if (spaceLoaded) ui->startStopButton_2->setEnabled(true);
}

void MainWindow::updateListenerPos() {
  ui->listenerPosLabel->setText(
      ("Listener position: " + space.listenerPositionStr()).c_str());
}

void MainWindow::updateListenerAngle() {
  ui->listenerAngleLabel->setText(
      ("Listener angle: " + space.listenerDirectionStr()).c_str());
}
