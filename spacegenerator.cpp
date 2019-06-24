#include "spacegenerator.h"
#include <random>
#include <vector>

void SpaceGenerator::generate() {
  generatedSpace = Space();
  generatedSpace->createEmptySpace(generatedFieldSize, generatedFieldSize);
  unsigned int numberOfFields = generatedFieldSize * generatedFieldSize;
  unsigned int nonEmptyFields = 0;

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

  Coordinates startPlace = findStartPlace();

  mapWidget->loadMap(generatedSpace->getFields());
  mapWidget->setPlayerCoordinates(startPlace);
  mapWidget->show();
}

void SpaceGenerator::saveGeneratedSpace(const std::string &fileName) {
  if (!generatedSpace) {
    throw std::logic_error("Cannot save space which is not generated yet");
  }
  std::ofstream file;
  file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
  file.open(fileName.c_str());
  generatedSpace->saveToTextFile(file);
}

void SpaceGenerator::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

Coordinates SpaceGenerator::findStartPlace() {
  while (true) {
    auto x = distribution(randomGenerator);
    auto y = distribution(randomGenerator);
    if (generatedSpace->getFields()[y][x].height() == 0) {
      return Coordinates(x, 0, y);
    }
  }
}
