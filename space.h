#ifndef SPACE_H
#define SPACE_H

#include "coordinates.h"
#include "newspaceplayer.h"
#include "simplespaceobject.h"
#include "viewingcone.h"
#include "mapwidget.h"
#include <vector>

class InvalidSpaceFile : public std::exception {};

class Space
{
public:
    Space(int updateFreq);
    void loadFromFile(const char* fname);
    Coordinates getInitListenerPos();
    std::string getName() { return name; }

    void setMapWidget(MapWidget* mapWidget) { this->mapWidget = mapWidget; }

    void rotateListenerLeft(float time);
    void rotateListenerRight(float time);
    void goForward(float time);
    void goBackward(float time);
    void printVisibleObjects();

    void startPlaying();
    void stopPlaying();
    void update();
    void setAngleX(const Angle &angleX);

    std::string listenerPositionStr() { return cone.getPosition().str(); }
    std::string listenerDirectionStr() { return cone.getDirection().str(); }

    void setFieldsVisibility();
    void setFieldsFocus();
    void updateClosestFocusField();
    void toggleMovingFocusAngle();

    bool lookingAt(const SimpleSpaceObject &object);

    bool firstCloser(const SimpleSpaceObject &first, const SimpleSpaceObject &second);
    float distanceSqFrom(SimpleSpaceObject obj);

    std::vector<std::vector<SimpleSpaceObject>>& getFields() { return fields; }
    Coordinates getPlayerPosition() { return cone.getPosition(); }

private:
    void moveFocusAngle();
    void playClosestFocusField();

    int updateFreq;
    std::string name;
    std::vector<std::vector<SimpleSpaceObject>> fields;
    ViewingCone cone;
    float focusAngleVal;
    float focusAngleMoveSpeed;  // deg/s
    SimpleSpaceObject closestField;
    NewSpacePlayer *sp;
    bool closestFieldExists;
    bool closestFieldChanged;
    bool movingFocusAngle;

public:
    MapWidget* mapWidget;
};

#endif // SPACE_H
