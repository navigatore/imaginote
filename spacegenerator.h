#pragma once
#include <optional>
#include "mapwidget.h"
#include "space.h"

class SpaceGenerator {
 public:
  SpaceGenerator() = default;

  void generate();
  void saveGeneratedSpace(const std::string& fileName);
  void setMapWidget(MapWidget* mapWidget);

 private:
  static constexpr unsigned int generatedFieldSize = 5;
  static constexpr float nonEmptyArea = 0.2F;
  static constexpr unsigned int maxHeight = 3;

  Coordinates findStartPlace();

  std::optional<Space> generatedSpace;
  MapWidget* mapWidget{};

  std::random_device randomDevice;
  std::mt19937 randomGenerator{randomDevice()};
  std::uniform_int_distribution<unsigned long> distribution{
      0, generatedFieldSize - 1};
  std::uniform_int_distribution<unsigned int> heightDistribution{1, maxHeight};
};
