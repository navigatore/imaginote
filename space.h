#ifndef SPACE_H
#define SPACE_H

#include "coordinates.h"
#include "simplespaceobject.h"
#include "viewingcone.h"
#include <vector>

class InvalidSpaceFile : public std::exception {};

class Space
{
public:
    Space() : cone(Angle(90.0f), Angle(180.0f), Angle(180.0f), 1000.0f) { }
    void loadFromFile(const char* fname);
    Coordinates getInitListenerPos();
    std::string getName() { return name; }

    void rotateListenerLeft(float time) { cone.rotateLeft(time); }
    void rotateListenerRight(float time) { cone.rotateRight(time); }
    void goForward(float time) { cone.forward(time); }
    void goBackward(float time) { cone.backward(time); }
    void printVisibleObjects();

    std::string listenerPositionStr() { return cone.getPosition().str(); }
    std::string listenerDirectionStr() { return cone.getDirection().str(); }

private:
    std::string name;
    std::vector<std::vector<SimpleSpaceObject>> fields;
    ViewingCone cone;
};

#endif // SPACE_H
