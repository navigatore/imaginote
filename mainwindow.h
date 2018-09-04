#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "angle.h"
#include "space.h"
#include "spaceplayer.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadSpaceDef();
    void startStopClicked();
    void validatePresInterval(const QString &text);
    void update();

protected:
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

private:
    const int updateFreq = 60;  // 60 Hz
    const float speed = 1.0f;        // 1 m/s
    const float angleSpeed = 45.0f;  // 45 deg/s

    void tryEnableStartStop();
    bool is_number(const std::string& s);
    void updateListenerPos();
    void updateListenerAngle();

    Ui::MainWindow *ui;

    QTimer *timer;

    bool playing;

    bool keyLeftPressed, keyRightPressed, keyUpPressed, keyDownPressed;

    Space space;
    SpacePlayer *sp;

    Coordinates listenerPos;
    Angle listenerAngle;

    bool spaceLoaded;
    bool presIntValid;
};

#endif // MAINWINDOW_H
