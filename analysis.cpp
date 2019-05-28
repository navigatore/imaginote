#include "analysis.h"
#include <chrono>
#include <fstream>

Analysis::Analysis(std::chrono::milliseconds updatePeriod)
    : track(updatePeriod) {}

void Analysis::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

void Analysis::loadRecording(const std::string &filename) {
  try {
    fields.clear();
    std::ifstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    f.open(filename.c_str());
    uint32_t fixedWidth = 0;
    f.read(reinterpret_cast<char *>(&fixedWidth), sizeof(fixedWidth));
    auto width = static_cast<unsigned int>(fixedWidth);
    uint32_t fixedHeight = 0;
    f.read(reinterpret_cast<char *>(&fixedHeight), sizeof(fixedHeight));
    auto height = static_cast<unsigned int>(fixedHeight);

    for (unsigned int z = 0; z < height; ++z) {
      fields.emplace_back();
      for (unsigned int x = 0; x < width; ++x) {
        uint32_t fixedFieldHeight = 0;
        f.read(reinterpret_cast<char *>(&fixedFieldHeight),
               sizeof(fixedFieldHeight));
        auto fieldHeight = static_cast<unsigned int>(fixedFieldHeight);
        auto floatX = static_cast<float>(x);
        auto floatZ = static_cast<float>(z);
        auto obj = SimpleSpaceObject(Coordinates(floatX, 0, floatZ),
                                     fieldHeight, fieldHeight > 0);
        fields[z].push_back(obj);
      }
    }

    track.load(f);
    f.close();

    mapWidget->loadMap(fields);
    mapWidget->setTrack(track);
    mapWidget->show();
  } catch (std::ifstream::failure &) {
    mapWidget->hide();
    throw InvalidFile();
  }
}
