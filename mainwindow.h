#pragma once
#include <QMainWindow>
#include "analysis.h"
#include "angle.h"
#include "simulationcontroller.h"
#include "spacegenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 public slots:
  void loadSpaceDef();
  void loadTrack();
  void generateSpace();
  void saveSpace();
  void startStopClicked();
  void update();

 protected:
  void keyPressEvent(QKeyEvent *ev);
  void keyReleaseEvent(QKeyEvent *ev);

 private:
  static const unsigned int updateFreq = 60;
  static constexpr std::chrono::milliseconds updatePeriod{1000 / updateFreq};

  void tryEnableStartStop();
  bool is_number(const std::string &s);
  void updateListenerPos();
  void updateListenerAngle();
  void updateVolume();
  void updateAnalysisLabels();

  void startClicked();
  void stopClicked();

  Ui::MainWindow *ui;

  QTimer *timer;

  bool playing;

  bool keyLeftPressed = false, keyRightPressed = false, keyUpPressed = false,
       keyDownPressed = false;

  SimulationController space;
  Analysis analysis;
  SpaceGenerator spaceGenerator;

  bool spaceLoaded;
  bool presIntValid;
};
