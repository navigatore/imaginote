#ifndef SPACE_H
#define SPACE_H

#include "coordinates.h"
#include "newspaceplayer.h"
#include "simplespaceobject.h"
#include "viewingcone.h"
#include <vector>

class InvalidSpaceFile : public std::exception {};

class Space
{
public:
    Space() : cone(Angle(90.0f), Angle(180.0f), Angle(180.0f), 1000.0f), closestField(0.0f, 0.0f, 0.0f), sp(nullptr) { }
    void loadFromFile(const char* fname);
    Coordinates getInitListenerPos();
    std::string getName() { return name; }

    void rotateListenerLeft(float time);
    void rotateListenerRight(float time);
    void goForward(float time) { cone.forward(time); printVisibleObjects(); }
    void goBackward(float time) { cone.backward(time); printVisibleObjects(); }
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

    bool lookingAt(const SimpleSpaceObject &object);

    bool firstCloser(const SimpleSpaceObject &first, const SimpleSpaceObject &second);
    float distanceSqFrom(SimpleSpaceObject obj);

private:
    std::string name;
    std::vector<std::vector<SimpleSpaceObject>> fields;
    ViewingCone cone;
    Angle focusAngle;
    SimpleSpaceObject closestField;
    NewSpacePlayer *sp;
    bool closestFieldExists;
};

#endif // SPACE_H
