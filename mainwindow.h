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
  void updateGeneratedWidth(int width);
  void updateGeneratedHeight(int height);
  void showHelp();

 protected:
  void keyPressEvent(QKeyEvent *ev);
  void keyReleaseEvent(QKeyEvent *ev);

 private:
  void tryEnableStartStop();
  bool is_number(const std::string &s);
  void updateListenerPos();
  void updateListenerAngle();
  void updateVolume();
  void updateAnalysisLabels();
  void updateExtendedAnalysisLabels();

  void startClicked();
  void stopClicked();

  void showAnalysis();
  void hideAnalysis();

  Ui::MainWindow *ui;

  QTimer *timer;

  bool playing;

  bool keyLeftPressed{};
  bool keyRightPressed{};
  bool keyUpPressed{};
  bool keyDownPressed{};

  SimulationController simulationController;
  Analysis analysis;
  SpaceGenerator spaceGenerator;

  bool spaceLoaded;
  bool presIntValid;
  unsigned int generatedWidth{5};
  unsigned int generatedHeight{5};
};
