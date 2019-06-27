#include "spacegenerator.h"
#include <random>
#include <vector>

void SpaceGenerator::generate(unsigned int width, unsigned int height) {
  std::uniform_int_distribution<unsigned long> xAxisDistribution(1, width - 2);
  std::uniform_int_distribution<unsigned long> zAxisDistribution(1, height - 2);
  generatedSpace = Space();
  generatedSpace->createEmptySpace(width, height);
  addBorderWithOneExit();
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
  generatedSpace->setStartPosition(startPlace);

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

void SpaceGenerator::addBorderWithOneExit() {
  unsigned int borderHeight = 3;
  for (unsigned int i = 0; i < generatedSpace->getFieldsWidth(); ++i) {
    generatedSpace->setFieldHeight(i, 0, borderHeight);
    generatedSpace->setFieldHeight(i, generatedSpace->getFieldsHeight() - 1,
                                   borderHeight);
  }
  for (unsigned int i = 1; i < generatedSpace->getFieldsHeight() - 1; ++i) {
    generatedSpace->setFieldHeight(0, i, borderHeight);
    generatedSpace->setFieldHeight(generatedSpace->getFieldsWidth() - 1, i,
                                   borderHeight);
  }
  auto edge = tldr(randomGenerator);
  unsigned int x{}, z{};
  std::uniform_int_distribution<unsigned long> widthDistribution(
      1, generatedSpace->getFieldsWidth() - 2);
  std::uniform_int_distribution<unsigned long> heightDistribution(
      1, generatedSpace->getFieldsHeight() - 2);

  switch (edge) {
    case 0:
      x = widthDistribution(randomGenerator);
      z = 0;
      break;

    case 1:
      z = heightDistribution(randomGenerator);
      x = 0;
      break;

    case 2:
      x = widthDistribution(randomGenerator);
      z = generatedSpace->getFieldsHeight() - 1;
      break;

    case 3:
      z = heightDistribution(randomGenerator);
      x = generatedSpace->getFieldsWidth() - 1;
  }
  generatedSpace->setFieldHeight(x, z, 0);
}
