#pragma once
#include <vector>
#include "coordinates.h"
#include "filenamestring.h"
#include "mapwidget.h"
#include "simplespaceplayer.h"
#include "recording.h"
#include "simplespaceobject.h"
#include "space.h"
#include "track.h"
#include "viewingcone.h"

class InvalidSpaceFile : public std::exception {};

class SimulationController {
 public:
  SimulationController() = default;

  static constexpr float fieldSize = 1.0F;
  static constexpr float halfFieldSize = fieldSize / 2.0F;
  static constexpr float volumeMultiplierChangeStep = 0.1F;

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

  void setRecordingActivated(bool activated);

  [[nodiscard]] bool getExitReached() const noexcept;
  [[nodiscard]] bool getRecordingEnabled() const noexcept;
  bool firstCloser(const SimpleSpaceObject &first,
                   const SimpleSpaceObject &second);
  float distanceSqFrom(SimpleSpaceObject obj);

  std::vector<std::vector<SimpleSpaceObject>> &getFields();
  [[nodiscard]] Coordinates getPlayerPosition() const;
  [[nodiscard]] float getVolume() const;

 private:
  void moveFocusAngle(float time);
  void playClosestFocusField();
  void saveRecording();

  [[nodiscard]] bool canGoInto(const Coordinates &point) const;
  [[nodiscard]] bool outOfMap() const noexcept;

  FileNameString name;
  Space space;
  std::optional<ViewingCone> cone;
  Coordinates startPos;
  std::optional<SimpleSpaceObject> closestField;
  SimpleSpaceObject *standingField{};
  GenericSpacePlayer *sp{};
  MapWidget *mapWidget{};
  std::unique_ptr<Recording> recording;
  float volumeMultiplier{1.0F};
  bool exitReached{};
  bool closestFieldChanged{false};
  bool movingFocusAngle{false};
  bool recordingEnabled{false};
};
