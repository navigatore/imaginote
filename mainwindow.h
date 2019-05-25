#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "angle.h"
#include "space.h"

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
  void startStopClicked();
  void update();

 protected:
  void keyPressEvent(QKeyEvent *ev);
  void keyReleaseEvent(QKeyEvent *ev);

 private:
  const int updateFreq = 60;  // 60 Hz

  void tryEnableStartStop();
  bool is_number(const std::string &s);
  void updateListenerPos();
  void updateListenerAngle();
  void updateVolume();

  void startClicked();
  void stopClicked();

  Ui::MainWindow *ui;

  QTimer *timer;

  bool playing;

  bool keyLeftPressed, keyRightPressed, keyUpPressed, keyDownPressed;

  Space space;

  bool spaceLoaded;
  bool presIntValid;
};

#endif  // MAINWINDOW_H
