#include "mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "sonarspaceplayer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      space(updatePeriod),
      analysis(updatePeriod),
      spaceLoaded(false) {
  ui->setupUi(this);
  ui->spaceLabel->setText("No space definition loaded");
  playing = false;
  presIntValid = true;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(updatePeriod);

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

  space.setMapWidget(ui->mapWidget);
  analysis.setMapWidget(ui->analysisMapWidget);
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
    updateVolume();

    if (space.outOfMap()) {
      stopClicked();
      space.setFromBeginning();
    }
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
  } else if (ev->key() == Qt::Key_P) {
    if (playing) {
      space.volumeUp();
    }
  } else if (ev->key() == Qt::Key_L) {
    if (playing) {
      space.volumeDown();
    }
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
  ui->startStopButton->setEnabled(false);
  spaceLoaded = false;

  QString fileName = QFileDialog::getOpenFileName(
      this, "Select space definition file", QDir::currentPath(),
      "Space definition (*.spc)");

  QByteArray ba = fileName.toLatin1();
  const char *c_str = ba.data();

  try {
    space.loadFromFile(c_str);
    ui->spaceLabel->setText(space.getName().c_str());
    spaceLoaded = true;
    tryEnableStartStop();
  } catch (InvalidSpaceFile &) {
    ui->spaceLabel->setText("Invalid file!");
  }
}

void MainWindow::loadTrack() {
  QString qFileName = QFileDialog::getOpenFileName(
      this, "Select recorded track file", QDir::currentPath(),
      "Imaginote track (*.rec)");
  if (qFileName.length() == 0) {
    return;
  }
  std::string filename(qFileName.toLatin1().data());
  std::string shortFilename;
  if (filename.find('/') == std::string::npos) {
    shortFilename = filename;
  } else {
    shortFilename = filename.substr(filename.rfind('/') + 1);
  }

  try {
    analysis.loadRecording(filename);
    ui->trackNameLabel->setText(shortFilename.c_str());
  } catch (Analysis::InvalidFile &) {
    ui->trackNameLabel->setText("Invalid file!");
  }
}

void MainWindow::startStopClicked() {
  if (!playing) {
    startClicked();
  } else {
    stopClicked();
  }
}

void MainWindow::startClicked() {
  ui->startStopButton->setText("Stop");

  GenericSpacePlayer *player = nullptr;
  if (ui->simpleButton->isChecked()) {
    player = new NewSpacePlayer();
  } else if (ui->sonarButton->isChecked()) {
    player = new SonarSpacePlayer();
  }

  ui->visualAngleSlider->setEnabled(false);
  ui->distanceLimitSlider->setEnabled(false);
  ui->loadSpaceButton->setEnabled(false);
  ui->recordTrackCheckBox->setEnabled(false);
  for (int i = 0; i < ui->tabWidget->count(); ++i) {
    if (i != ui->tabWidget->currentIndex()) {
      ui->tabWidget->setTabEnabled(i, false);
    }
  }

  playing = true;
  auto angleX = Angle(ui->visualAngleSlider->value());
  auto maxDistance = ui->distanceLimitSlider->value();
  space.setRecording(ui->recordTrackCheckBox->isChecked());
  space.startPlaying(angleX, maxDistance, player);
  adjustSize();
}

void MainWindow::stopClicked() {
  ui->startStopButton->setText("Start");
  ui->visualAngleSlider->setEnabled(true);
  ui->distanceLimitSlider->setEnabled(true);
  ui->loadSpaceButton->setEnabled(true);
  ui->recordTrackCheckBox->setEnabled(true);
  for (int i = 0; i < ui->tabWidget->count(); ++i) {
    ui->tabWidget->setTabEnabled(i, true);
  }

  playing = false;
  space.stopPlaying();
  if (space.recordingEnabled()) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream filename;
    filename << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".rec";
    space.saveRecording(std::string((filename.str())));
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
  if (spaceLoaded) ui->startStopButton->setEnabled(true);
}

void MainWindow::updateListenerPos() {
  ui->listenerPosLabel->setText(
      ("Listener position: " + space.listenerPositionStr()).c_str());
}

void MainWindow::updateListenerAngle() {
  ui->listenerAngleLabel->setText(
      ("Listener angle: " + space.listenerDirectionStr()).c_str());
}

void MainWindow::updateVolume() {
  if (!playing) {
    ui->volumeLabel->setText("Volume: undefined");
  } else {
    ui->volumeLabel->setText(
        ("Volume: " + std::to_string(space.getVolume())).c_str());
  }
}
