#include "mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "constants.h"
#include "keyboardlayouthelpdialog.h"
#include "sonarspaceplayer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), spaceLoaded(false) {
  ui->setupUi(this);
  ui->spaceLabel->setText("No space definition loaded");
  ui->listenerPosLabel->hide();
  ui->listenerAngleLabel->hide();
  hideAnalysis();
  playing = false;
  presIntValid = true;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(updatePeriod);

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

  simulationController.setMapWidget(ui->mapWidget);
  analysis.setMapWidget(ui->analysisMapWidget);
  spaceGenerator.setMapWidget(ui->generatorMapWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::update() {
  if (playing) {
    if (keyLeftPressed) {
      simulationController.rotateListenerLeft(1.0f / updateFreq);
    } else if (keyRightPressed) {
      simulationController.rotateListenerRight(1.0f / updateFreq);
    }
    if (keyUpPressed) {
      simulationController.goForward(1.0f / updateFreq);
    } else if (keyDownPressed) {
      simulationController.goBackward(1.0f / updateFreq);
    }
    simulationController.update(1.0f / updateFreq);
    updateListenerPos();
    updateListenerAngle();
    updateVolume();

    if (simulationController.getExitReached()) {
      stopClicked();
    }
  }
}

void MainWindow::updateGeneratedWidth(int width) {
  generatedWidth = static_cast<unsigned int>(width);
}

void MainWindow::updateGeneratedHeight(int height) {
  generatedHeight = static_cast<unsigned int>(height);
}

void MainWindow::showHelp() {
  auto helpDialog = new KeyboardLayoutHelpDialog(this);
  helpDialog->show();
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
    simulationController.toggleMovingFocusAngle();
  } else if (ev->key() == Qt::Key_H) {
    if (playing) {
      simulationController.toggleMapWidgetVisibility();
      ui->listenerPosLabel->setVisible(!ui->listenerPosLabel->isVisible());
      ui->listenerAngleLabel->setVisible(!ui->listenerAngleLabel->isVisible());
    }
  } else if (ev->key() == Qt::Key_P) {
    if (playing) {
      simulationController.volumeUp();
    }
  } else if (ev->key() == Qt::Key_L) {
    if (playing) {
      simulationController.volumeDown();
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

  if (!fileName.isEmpty()) {
    QByteArray ba = fileName.toLatin1();
    const char *c_str = ba.data();

    try {
      simulationController.loadFromFile(c_str);
      ui->spaceLabel->setText(simulationController.getName().c_str());
      spaceLoaded = true;
      tryEnableStartStop();
    } catch (InvalidSpaceFile &) {
      ui->spaceLabel->setText("Invalid file!");
    }
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
    updateAnalysisLabels();
    showAnalysis();
  } catch (Analysis::InvalidFile &) {
    ui->trackNameLabel->setText("Invalid file!");
    hideAnalysis();
  }
}

void MainWindow::generateSpace() {
  spaceGenerator.generate(generatedWidth, generatedHeight);
  ui->generatorMapWidget->repaint();
}

void MainWindow::saveSpace() {
  auto fileName = QFileDialog::getSaveFileName(
      this, "Enter the name of the file", QDir::currentPath(),
      "Imaginote space (*.spc)");
  if (!fileName.isEmpty()) {
    spaceGenerator.saveGeneratedSpace(std::string(fileName.toUtf8()));
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
  simulationController.setRecordingActivated(
      ui->recordTrackCheckBox->isChecked());
  simulationController.startPlaying(angleX, maxDistance, player);
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
  simulationController.stopPlaying();
}

void MainWindow::showAnalysis() {
  ui->analysisMapWidget->show();
  ui->timeDurationLabel->show();
  ui->meanDifferenceLabel->show();
  ui->exitReachedLabel->show();
  ui->sonificationMethodNameLabel->show();
  ui->visualAngleLabel_2->show();
  ui->distanceLimitLabel_2->show();
  ui->trackingRadiusTimeLabel->show();
  ui->mapPreviewTimeLabel->show();
  ui->spaceNameLabel->show();
}

void MainWindow::hideAnalysis() {
  ui->analysisMapWidget->hide();
  ui->timeDurationLabel->hide();
  ui->meanDifferenceLabel->hide();
  ui->exitReachedLabel->hide();
  ui->sonificationMethodNameLabel->hide();
  ui->visualAngleLabel_2->hide();
  ui->distanceLimitLabel_2->hide();
  ui->trackingRadiusTimeLabel->hide();
  ui->mapPreviewTimeLabel->hide();
  ui->spaceNameLabel->hide();
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
      ("Listener position: " + simulationController.listenerPositionStr())
          .c_str());
}

void MainWindow::updateListenerAngle() {
  ui->listenerAngleLabel->setText(
      ("Listener angle: " + simulationController.listenerDirectionStr())
          .c_str());
}

void MainWindow::updateVolume() {
  if (!playing) {
    ui->volumeLabel->setText("Volume: undefined");
  } else {
    ui->volumeLabel->setText(
        ("Volume: " + std::to_string(simulationController.getVolume()))
            .c_str());
  }
}

void MainWindow::updateAnalysisLabels() {
  ui->timeDurationLabel->setText(
      (std::string("Duration: ") + analysis.getDuration().toString()).c_str());
  ui->meanDifferenceLabel->setText(
      (std::string("Mean difference: ") +
       std::to_string(analysis.getMeanDifference()) + " m")
          .c_str());
  ui->exitReachedLabel->setText("Exit reached: unknown");
  ui->sonificationMethodNameLabel->setText("Sonification method: unknown");
  ui->visualAngleLabel_2->setText("Visual angle: unknown");
  ui->distanceLimitLabel_2->setText("Distance limit: unknown");
  ui->trackingRadiusTimeLabel->setText("Moving tracking radius time: unknown");
  ui->mapPreviewTimeLabel->setText("Map preview enabled time: unknown");
  ui->spaceNameLabel->setText("Space name: unknown");
  if (analysis.getIsExtended()) {
    updateExtendedAnalysisLabels();
  }
}

void MainWindow::updateExtendedAnalysisLabels() {
  ui->exitReachedLabel->setText((std::string("Exit reached: ") +
                                 (analysis.getExitReached() ? "Yes" : "No"))
                                    .c_str());
  ui->sonificationMethodNameLabel->setText(
      (std::string("Sonification method: ") +
       analysis.getSonificationMethodName())
          .c_str());
  ui->visualAngleLabel_2->setText(
      (std::string("Visual angle: ") + analysis.getVisualAngle().str())
          .c_str());
  ui->distanceLimitLabel_2->setText(
      (std::string("Distance limit: ") +
       std::to_string(analysis.getDistanceLimit()) + " m")
          .c_str());
  ui->trackingRadiusTimeLabel->setText(
      (std::string("Moving tracking radius time: ") +
       std::to_string(analysis.getTrackingRadiusEnabledPercentageTime()) +
       std::string(" %"))
          .c_str());
  ui->mapPreviewTimeLabel->setText(
      (std::string("Map preview enabled time: ") +
       std::to_string(analysis.getMapPreviewEnabledPercentageTime()) +
       std::string(" %"))
          .c_str());
  ui->spaceNameLabel->setText(
      (std::string("Space name: ") + analysis.getSpaceName()).c_str());
}
