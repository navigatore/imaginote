#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QtGui>
#include "angle.h"
#include "simplespaceobject.h"
#include "track.h"

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MapWidget(QWidget* parent = nullptr);
  ~MapWidget();

  void loadMap(const std::vector<std::vector<SimpleSpaceObject>>& map);
  void unloadMap();
  void update(const Coordinates& playerCrds, const Angle& directionAngle,
              const Angle& focusAngle, const SimpleSpaceObject* closestField);
  void setAngleX(const Angle& angleX);
  void setDistanceLimit(float limit);
  void setTrack(const Track& track);

 protected:
  void paintEvent(QPaintEvent*);

 private:
  void paintFocusAngle();
  void paintPlayerAngle(Angle angle, const QColor& color);
  void paintClosestField();
  void paintFields();
  void setPenColor(const QColor& color);
  void paintPlayer();
  void paintDistanceLimitArc();
  void paintTrack();

  int calcPxPositionX(const Coordinates2d& crds);
  int calcPxPositionY(const Coordinates2d& crds);

  const int fieldSize = 30;
  const int playerFieldRadius = 2;

  Ui::MapWidget* ui;
  QPainter* painter;
  std::vector<std::vector<SimpleSpaceObject>> fields;
  const Track* track;
  bool mapLoaded;
  Angle directionAngle, angleX, focusAngle;
  float distanceLimit;
  const SimpleSpaceObject* closestField;
  int pxWidth, pxHeight;
  int playerPxPosX, playerPxPosY;
};

#endif  // MAPWIDGET_H
