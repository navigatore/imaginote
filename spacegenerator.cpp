#include "spacegenerator.h"
#include <random>
#include <vector>

void SpaceGenerator::generate() {
  generatedSpace = Space();
  generatedSpace->createEmptySpace(generatedFieldSize, generatedFieldSize);
  unsigned int numberOfFields = generatedFieldSize * generatedFieldSize;
  unsigned int nonEmptyFields = 0;

  std::random_device randomDevice;
  std::mt19937 randomGenerator(randomDevice());
  std::uniform_int_distribution<unsigned long> distribution(
      0, generatedFieldSize - 1);
  std::uniform_int_distribution<unsigned int> heightDistribution(1, maxHeight);

  while (static_cast<float>(nonEmptyFields) /
             static_cast<float>(numberOfFields) <
         nonEmptyArea) {
    auto x = distribution(randomGenerator);
    auto y = distribution(randomGenerator);
    if (generatedSpace->getFields()[y][x].height() > 0) {
      continue;
    }
    auto height = heightDistribution(randomGenerator);
    generatedSpace->getFields()[y][x].height() = height;
    ++nonEmptyFields;
  }
  mapWidget->loadMap(generatedSpace->getFields());
  mapWidget->show();
}

void SpaceGenerator::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}
