#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "angle.h"
#include "space.h"
#include "newspaceplayer.h"
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
    void startStopOneClicked();
    void validatePresInterval(const QString &text);
    void update();

protected:
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

private:
    const int updateFreq = 60;  // 60 Hz

    void tryEnableStartStop();
    bool is_number(const std::string& s);
    void updateListenerPos();
    void updateListenerAngle();
    void printDebugInfo();


    Ui::MainWindow *ui;

    QTimer *timer;

    bool playing;

    bool keyLeftPressed, keyRightPressed, keyUpPressed, keyDownPressed, printKeyPressed;

    Space space;
    NewSpacePlayer *sp;

    bool spaceLoaded;
    bool presIntValid;
};

#endif // MAINWINDOW_H
