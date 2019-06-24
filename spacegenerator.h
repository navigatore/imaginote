#ifndef SPACEGENERATOR_H
#define SPACEGENERATOR_H

#include <optional>
#include "mapwidget.h"
#include "space.h"

class SpaceGenerator {
 public:
  SpaceGenerator() = default;

  void generate();
  void setMapWidget(MapWidget* mapWidget);

 private:
  static constexpr unsigned int generatedFieldSize = 5;
  static constexpr float nonEmptyArea = 0.2F;
  static constexpr unsigned int maxHeight = 3;
  std::optional<Space> generatedSpace;
  MapWidget* mapWidget{};
};

#endif  // SPACEGENERATOR_H
