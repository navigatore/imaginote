#include "mapwidget.h"
#include <cmath>
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::MapWidget),
      painter(nullptr),
      mapLoaded(false),
      closestField(nullptr),
      pxWidth(0),
      pxHeight(0) {
  ui->setupUi(this);
}

MapWidget::~MapWidget() { delete ui; }

void MapWidget::update(const Coordinates& playerCrds,
                       const Angle& directionAngle, const Angle& focusAngle,
                       const SimpleSpaceObject* closestField) {
  playerPxPosX = static_cast<int>(fieldSize * (playerCrds.x + 0.5f));
  playerPxPosY = static_cast<int>(fieldSize * (playerCrds.z + 0.5f));
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
}

void MapWidget::unloadMap() {
  mapLoaded = false;
  setMinimumSize(0, 0);
  repaint();
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
      if (field.height > 0) {
        auto x = static_cast<int>(field.crds.x * fieldSize);
        auto y = static_cast<int>(field.crds.z * fieldSize);
        painter->drawRect(x, y, fieldSize - 2, fieldSize - 2);
        painter->drawText(x + fieldSize / 2, y + fieldSize / 2,
                          QString(std::to_string(field.height).c_str()));
      }
    }
  }
}

void MapWidget::paintPlayer() {
  int playerBeginX = playerPxPosX - playerFieldRadius;
  int playerBeginY = playerPxPosY - playerFieldRadius;
  int size = playerFieldRadius * 2 + 1;
  painter->fillRect(playerBeginX, playerBeginY, size, size,
                    QColor(255, 255, 0));
  painter->drawRect(playerBeginX, playerBeginY, size, size);
}

void MapWidget::paintEvent(QPaintEvent*) {
  if (mapLoaded) {
    QPainter painter(this);
    this->painter = &painter;
    paintCone();
    paintFocusAngle();
    paintClosestField();
    paintFields();
    paintPlayer();
    this->painter = nullptr;
  }
}

void MapWidget::paintFocusAngle() {
  paintPlayerAngle(focusAngle, QColor(255, 0, 0));
}

void MapWidget::paintCone() {
  auto color = QColor(0, 255, 0);
  paintPlayerAngle(directionAngle - angleX, color);
  paintPlayerAngle(directionAngle + angleX, color);
}

void MapWidget::paintPlayerAngle(Angle angle, const QColor& color) {
  float radius =
      static_cast<float>(std::sqrt(pxWidth * pxWidth + pxHeight * pxHeight));
  int x = static_cast<int>(radius * std::cos(angle.getRad()));
  int y = static_cast<int>(radius * std::sin(angle.getRad()));

  QPen pen;
  pen.setColor(color);
  painter->setPen(pen);
  painter->drawLine(playerPxPosX, playerPxPosY, playerPxPosX + x,
                    playerPxPosY - y);
}

void MapWidget::paintClosestField() {
  if (closestField != nullptr) {
    int x = static_cast<int>(closestField->crds.x) * fieldSize;
    int y = static_cast<int>(closestField->crds.z) * fieldSize;

    painter->fillRect(x, y, fieldSize - 2, fieldSize - 2, QColor(255, 0, 0));
  }
}

void MapWidget::setAngleX(const Angle& angleX) { this->angleX = angleX; }
