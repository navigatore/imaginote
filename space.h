#ifndef SPACE_H
#define SPACE_H

#include "coordinates.h"
#include <vector>

class InvalidSpaceFile : public std::exception {};

class Space
{
public:
    Space() { }
    void loadFromFile(const char* fname);
    Coordinates getInitListenerPos();
    std::string getName() { return name; }

private:
    std::string name;
    std::vector<std::vector<int>> fields;
    Coordinates initListenerPos;
};

#endif // SPACE_H
