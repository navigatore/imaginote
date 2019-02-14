#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include "simplespaceobject.h"


namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();
    void loadMap(const std::vector<std::vector<SimpleSpaceObject>>& map);
    void unloadMap();
    void update(Coordinates playerCrds);

protected:
    void paintEvent(QPaintEvent*);

private:
    const unsigned int fieldSize = 30;
    const unsigned int playerFieldRadius = 2;

    Ui::MapWidget *ui;
    std::vector<std::vector<SimpleSpaceObject>> fields;
    Coordinates playerCrds;
    bool mapLoaded;
};

#endif // MAPWIDGET_H
