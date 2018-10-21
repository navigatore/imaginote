#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include "coordinates.h"


class SimpleSpaceObject
{
public:
    SimpleSpaceObject(Coordinates crd, unsigned int height, bool visible = false) : crd(crd), height(height), visible(visible) { }

    Coordinates crd;
    unsigned int height;
    bool visible;
};

#endif // SIMPLESPACEOBJECT_H
