#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include "coordinates.h"


class SimpleSpaceObject
{
public:
    SimpleSpaceObject(Coordinates crd, unsigned int height, bool visible = false) : crds(crd), height(height), visible(visible) { }

    Coordinates crds;
    unsigned int height;
    bool visible;
};

#endif // SIMPLESPACEOBJECT_H
