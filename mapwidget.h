#pragma once
#include <QWidget>
#include <QtGui>
#include <optional>

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
  MapWidget(const MapWidget&) = delete;
  MapWidget(MapWidget&&) = delete;
  MapWidget& operator=(const MapWidget&) = delete;
  MapWidget& operator=(MapWidget&&) = delete;
  ~MapWidget() override;

  void loadMap(const std::vector<std::vector<SimpleSpaceObject>>& map);
  void unloadMap();
  void update(const Coordinates& playerCoordinates, const Angle& directionAngle,
              const Angle& focusAngle,
              const std::optional<SimpleSpaceObject>& closestField);
  void setPlayerCoordinates(const Coordinates& playerCoordinates);
  void setAngleX(const Angle& angleX);
  void setDistanceLimit(float limit);
  void setTrack(const Track& track);
  void setCorners(const std::vector<Coordinates2d>& points);
  void setExitCorners(const std::vector<Coordinates2d>& points);
  void setShortestPath(const std::vector<Coordinates2d>& points);

 protected:
  void paintEvent(QPaintEvent* /*event*/) override;

 private:
  void paintFocusAngle();
  void paintPlayerAngle(Angle angle, const QColor& color);
  void paintClosestField();
  void paintFields();
  void setPenColor(const QColor& color);
  void paintPlayer();
  void paintDistanceLimitArc();
  void paintTrack();
  void paintAllCorners();
  void paintExitCorners();
  void paintShortestPath();
  void paintCorners(const std::vector<Coordinates2d>* ptr, const QColor& color);

  int calcPxPositionX(const Coordinates2d& crds);
  int calcPxPositionY(const Coordinates2d& crds);

  const int fieldSize = 30;
  const int playerFieldRadius = 2;
  static constexpr float fieldHalf{0.5F};
  static constexpr QColor closestFieldColor{255, 0, 0};
  static constexpr QColor focusAngleColor{255, 0, 0};
  static constexpr QColor trackColor{255, 255, 0};
  static constexpr QColor cornersColor{0, 255, 255};
  static constexpr QColor exitCornersColor{255, 0, 0};
  static constexpr QColor fieldColor{255, 255, 255};
  static constexpr QColor distanceLimitArcColor{0, 255, 0};
  static constexpr QColor shortestPathColor{0, 255, 0};
  static constexpr QColor playerColor{255, 255, 0};
  static constexpr QColor visitedFieldColor{0, 100, 100};

  Ui::MapWidget* ui;
  QPainter* painter;
  const std::vector<std::vector<SimpleSpaceObject>>* fields{};
  const Track* track;
  std::vector<Coordinates2d> corners;
  std::vector<Coordinates2d> exitCorners;
  std::vector<Coordinates2d> shortestPathNodes;
  bool mapLoaded;
  bool simulationMode{};
  Angle directionAngle, angleX, focusAngle;
  std::optional<float> distanceLimit;
  std::optional<SimpleSpaceObject> closestField;
  int pxWidth, pxHeight;
  std::optional<int> playerPxPosX, playerPxPosY;
};
