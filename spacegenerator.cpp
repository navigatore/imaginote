#include "spacegenerator.h"
#include <random>
#include <vector>

void SpaceGenerator::generate(unsigned int width, unsigned int height) {
  std::uniform_int_distribution<unsigned long> xAxisDistribution(1, width - 2);
  std::uniform_int_distribution<unsigned long> zAxisDistribution(1, height - 2);
  generatedSpace = Space();
  generatedSpace->createEmptySpace(width, height);
  unsigned int numberOfFields = (width - 2) * (height - 2);
  unsigned int nonEmptyFields = 0;

  while (static_cast<float>(nonEmptyFields) /
             static_cast<float>(numberOfFields) <
         nonEmptyArea) {
    auto x = xAxisDistribution(randomGenerator);
    auto y = zAxisDistribution(randomGenerator);
    if (generatedSpace->getFields()[y][x].height() > 0) {
      continue;
    }
    auto height = heightDistribution(randomGenerator);
    generatedSpace->getFields()[y][x].height() = height;
    ++nonEmptyFields;
  }

  Coordinates startPlace = findStartPlace(xAxisDistribution, zAxisDistribution);

  mapWidget->loadMap(generatedSpace->getFields());
  mapWidget->setPlayerCoordinates(startPlace);
  mapWidget->show();
}

void SpaceGenerator::saveGeneratedSpace(const std::string& fileName) {
  if (!generatedSpace) {
    throw std::logic_error("Cannot save space which is not generated yet");
  }
  std::ofstream file;
  file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
  file.open(fileName.c_str());
  generatedSpace->saveToTextFile(file);
}

void SpaceGenerator::setMapWidget(MapWidget* mapWidget) {
  this->mapWidget = mapWidget;
}

Coordinates SpaceGenerator::findStartPlace(
    std::uniform_int_distribution<unsigned long>& xAxisDistribution,
    std::uniform_int_distribution<unsigned long>& yAxisDistribution) {
  while (true) {
    auto x = xAxisDistribution(randomGenerator);
    auto y = yAxisDistribution(randomGenerator);
    if (generatedSpace->getFields()[y][x].height() == 0) {
      return Coordinates(x, 0, y);
    }
  }
}
