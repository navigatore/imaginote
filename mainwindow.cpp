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
  } else if (ev->key() == Qt::Key_H) {
    space.toggleMapWidgetVisibility();
    ui->listenerPosLabel->setVisible(!ui->listenerPosLabel->isVisible());
    ui->listenerAngleLabel->setVisible(!ui->listenerAngleLabel->isVisible());
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
