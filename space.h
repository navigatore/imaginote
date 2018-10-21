#ifndef SPACE_H
#define SPACE_H

#include "coordinates.h"
#include "simplespaceobject.h"
#include <vector>

class InvalidSpaceFile : public std::exception {};

class Space
{
public:
    Space() { }
    void loadFromFile(const char* fname);
    Coordinates getInitListenerPos();
    std::string getName() { return name; }

    void printVisibleObjects();

private:
    std::string name;
    std::vector<std::vector<SimpleSpaceObject>> fields;
    Coordinates initListenerPos;
};

#endif // SPACE_H
