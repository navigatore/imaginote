#pragma once
#include <vector>
#include "coordinates.h"
#include "filenamestring.h"
#include "mapwidget.h"
#include "newspaceplayer.h"
#include "simplespaceobject.h"
#include "space.h"
#include "track.h"
#include "viewingcone.h"

class InvalidSpaceFile : public std::exception {};

class SimulationController {
 public:
  SimulationController(const std::chrono::milliseconds &updatePeriod);

  static constexpr float fieldSize = 1.0F;
  static constexpr float halfFieldSize = fieldSize / 2.0F;

  void loadFromFile(const char *fname);
  Coordinates getInitListenerPos();
  std::string getName();

  void clearState();
  void setFromBeginning();

  void setMapWidget(MapWidget *mapWidget);

  void rotateListenerLeft(float time);
  void rotateListenerRight(float time);
  void goForward(float time);
  void goBackward(float time);
  void printVisibleObjects();

  void startPlaying(Angle angleX, float maxDistance, GenericSpacePlayer *sp);
  void stopPlaying();
  void update(float time);

  std::string listenerPositionStr();
  std::string listenerDirectionStr();

  void setFieldsVisibility();
  void setFieldsFocus();
  void updateClosestFocusField();
  void updateStandingField();
  void toggleMovingFocusAngle();
  void toggleMapWidgetVisibility();

  void volumeUp();
  void volumeDown();

  void setRecording(bool activated);
  void saveRecording(const std::string &filename);

  [[nodiscard]] bool outOfMap() const;
  [[nodiscard]] bool recordingEnabled() const;
  bool firstCloser(const SimpleSpaceObject &first,
                   const SimpleSpaceObject &second);
  float distanceSqFrom(SimpleSpaceObject obj);

  std::vector<std::vector<SimpleSpaceObject>> &getFields();
  [[nodiscard]] Coordinates getPlayerPosition() const;
  [[nodiscard]] float getVolume() const;

 private:
  static constexpr uint32_t recordingMagicNumber = 0x84465e34;
  static constexpr uint32_t recordingVersion2Constant = 2;

  void moveFocusAngle(float time);
  void playClosestFocusField();
  [[nodiscard]] bool canGoInto(const Coordinates &point) const;

  FileNameString name;
  Space space;
  std::optional<ViewingCone> cone;
  Coordinates startPos;
  std::optional<SimpleSpaceObject> closestField;
  SimpleSpaceObject *standingField{};
  GenericSpacePlayer *sp{};
  Track recTrack;
  MapWidget *mapWidget{};
  bool closestFieldChanged{false};
  bool movingFocusAngle{false};
  bool recording{false};
};
