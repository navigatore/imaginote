#pragma once
#include <optional>

#include "mapwidget.h"
#include "space.h"

class SpaceGenerator {
 public:
  SpaceGenerator() = default;

  void generate(unsigned int width, unsigned int height);
  void saveGeneratedSpace(const std::string& fileName);
  void setMapWidget(MapWidget* mapWidget);

 private:
  static constexpr float nonEmptyArea = 0.2F;
  static constexpr unsigned int maxHeight = 3;

  Coordinates findStartPlace(
      std::uniform_int_distribution<unsigned long>& xAxisDistribution,
      std::uniform_int_distribution<unsigned long>& yAxisDistribution);

  void addBorderWithOneExit();

  std::optional<Space> generatedSpace;
  MapWidget* mapWidget{};

  std::random_device randomDevice;
  std::mt19937 randomGenerator{randomDevice()};

  std::uniform_int_distribution<unsigned int> fieldHeightDistribution{
      1, maxHeight};
  std::uniform_int_distribution<unsigned int> tldr{0, 3};
};
