#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include "coordinates.h"
#include "mapwidget.h"
#include "newspaceplayer.h"
#include "simplespaceobject.h"
#include "viewingcone.h"

class InvalidSpaceFile : public std::exception {};

class Space {
 public:
  Space(int updateFreq);
  void loadFromFile(const char *fname);
  Coordinates getInitListenerPos();
  std::string getName();

  void setMapWidget(MapWidget *mapWidget);

  void rotateListenerLeft(float time);
  void rotateListenerRight(float time);
  void goForward(float time);
  void goBackward(float time);
  void printVisibleObjects();

  void startPlaying(Angle angleX, float maxDistance, GenericSpacePlayer *sp);
  void stopPlaying();
  void update(float time);
  void setAngleX(const Angle &angleX);
  void setDistanceLimit(float limit);

  std::string listenerPositionStr();
  std::string listenerDirectionStr();

  void setFieldsVisibility();
  void setFieldsFocus();
  void updateClosestFocusField();
  void toggleMovingFocusAngle();
  void toggleMapWidgetVisibility();

  void volumeUp();
  void volumeDown();

  bool firstCloser(const SimpleSpaceObject &first,
                   const SimpleSpaceObject &second);
  float distanceSqFrom(SimpleSpaceObject obj);

  std::vector<std::vector<SimpleSpaceObject>> &getFields();
  Coordinates getPlayerPosition();
  float getVolume() const;

 private:
  void moveFocusAngle(float time);
  void playClosestFocusField();
  void clearState();
  bool canGoInto(const Coordinates &field) const;

  int updateFreq;
  std::string name;
  std::vector<std::vector<SimpleSpaceObject>> fields;
  ViewingCone cone;
  float focusAngleMoveSpeed;  // deg/s
  SimpleSpaceObject closestField;
  GenericSpacePlayer *sp;
  bool closestFieldExists;
  bool closestFieldChanged;
  bool movingFocusAngle;

 public:
  MapWidget *mapWidget;
};

#endif  // SPACE_H
