#include "mapwidget.h"
#include <cmath>
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::MapWidget),
      painter(nullptr),
      track(nullptr),
      mapLoaded(false),
      closestField(nullptr),
      pxWidth(0),
      pxHeight(0) {
  ui->setupUi(this);
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::black);
  setAutoFillBackground(true);
  setPalette(pal);
  hide();
}

MapWidget::~MapWidget() { delete ui; }

int MapWidget::calcPxPositionX(const Coordinates2d& crds) {
  return static_cast<int>(fieldSize * (crds.x() + 0.5f));
}

int MapWidget::calcPxPositionY(const Coordinates2d& crds) {
  return static_cast<int>(fieldSize * (crds.y() + 0.5f));
}

void MapWidget::update(const Coordinates& playerCrds,
                       const Angle& directionAngle, const Angle& focusAngle,
                       const SimpleSpaceObject* closestField) {
  playerPxPosX = calcPxPositionX(playerCrds);
  playerPxPosY = calcPxPositionY(playerCrds);

  this->directionAngle = directionAngle;
  this->focusAngle = focusAngle;
  this->closestField = closestField;
  repaint();
}

void MapWidget::loadMap(
    const std::vector<std::vector<SimpleSpaceObject>>& map) {
  fields = map;
  mapLoaded = true;
  pxWidth = fieldSize * static_cast<int>(fields[0].size());
  pxHeight = fieldSize * static_cast<int>(fields.size());
  setMinimumSize(pxWidth, pxHeight);
  setMaximumSize(9999, 9999);
  show();
}

void MapWidget::unloadMap() {
  mapLoaded = false;
  setMinimumSize(0, 0);
  setMaximumSize(0, 0);
  hide();
}

void MapWidget::setPenColor(const QColor& color) {
  QPen pen;
  pen.setColor(color);
  painter->setPen(pen);
}

void MapWidget::paintFields() {
  setPenColor(QColor(255, 255, 255));
  for (const auto& row : fields) {
    for (const auto& field : row) {
      if (field.height() > 0) {
        auto x = static_cast<int>(field.crds().x() * fieldSize);
        auto y = static_cast<int>(field.crds().z() * fieldSize);
        painter->drawRect(x, y, fieldSize - 2, fieldSize - 2);
        painter->drawText(x + fieldSize / 2, y + fieldSize / 2,
                          QString(std::to_string(field.height()).c_str()));
      }
    }
  }
}

void MapWidget::paintPlayer() {
  if (playerPxPosX) {
    int playerBeginX = *playerPxPosX - playerFieldRadius;
    int playerBeginY = *playerPxPosY - playerFieldRadius;
    int size = playerFieldRadius * 2 + 1;
    painter->fillRect(playerBeginX, playerBeginY, size, size,
                      QColor(255, 255, 0));
    painter->drawRect(playerBeginX, playerBeginY, size, size);
  }
}

void MapWidget::paintDistanceLimitArc() {
  if (distanceLimit) {
    auto distanceLimitRadiusPx = static_cast<int>(fieldSize * (*distanceLimit));
    setPenColor(QColor(0, 255, 0));
    painter->drawPie(*playerPxPosX - distanceLimitRadiusPx,
                     *playerPxPosY - distanceLimitRadiusPx,
                     2 * distanceLimitRadiusPx, 2 * distanceLimitRadiusPx,
                     (directionAngle - angleX).getQtAngle(),
                     (angleX * 2.0f).getQtAngle());
  }
}

void MapWidget::paintTrack() {
  if (track == nullptr) {
    return;
  }
  std::optional<std::pair<int, int>> lastCrds;
  setPenColor(QColor(255, 255, 0));
  for (auto& pos : track->getPositions()) {
    auto currCrds = std::make_pair(calcPxPositionX(pos), calcPxPositionY(pos));
    if (!lastCrds || currCrds != lastCrds) {
      if (lastCrds) {
        painter->drawLine(lastCrds->first, lastCrds->second, currCrds.first,
                          currCrds.second);
      }
      lastCrds = currCrds;
    }
  }
}

void MapWidget::paintEvent(QPaintEvent*) {
  if (mapLoaded) {
    QPainter painter(this);
    this->painter = &painter;
    paintFocusAngle();
    paintClosestField();
    paintFields();
    paintPlayer();
    paintDistanceLimitArc();
    paintTrack();
    this->painter = nullptr;
  }
}

void MapWidget::paintFocusAngle() {
  paintPlayerAngle(focusAngle, QColor(255, 0, 0));
}

void MapWidget::paintPlayerAngle(Angle angle, const QColor& color) {
  if (playerPxPosX) {
    float radius =
        static_cast<float>(std::sqrt(pxWidth * pxWidth + pxHeight * pxHeight));
    int x = static_cast<int>(radius * std::cos(angle.getRad()));
    int y = static_cast<int>(radius * std::sin(angle.getRad()));

    QPen pen;
    pen.setColor(color);
    painter->setPen(pen);
    painter->drawLine(*playerPxPosX, *playerPxPosY, *playerPxPosX + x,
                      *playerPxPosY - y);
  }
}

void MapWidget::paintClosestField() {
  if (closestField != nullptr) {
    int x = static_cast<int>(closestField->crds().x()) * fieldSize;
    int y = static_cast<int>(closestField->crds().z()) * fieldSize;

    painter->fillRect(x, y, fieldSize - 2, fieldSize - 2, QColor(255, 0, 0));
  }
}

void MapWidget::setAngleX(const Angle& angleX) { this->angleX = angleX; }

void MapWidget::setDistanceLimit(float limit) { distanceLimit = limit; }

void MapWidget::setTrack(const Track& track) { this->track = &track; }
