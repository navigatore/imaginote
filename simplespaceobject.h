#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include "coordinates.h"


class SimpleSpaceObject
{
public:
    SimpleSpaceObject(Coordinates crd, unsigned int height, bool visible = false) : crds(crd), height(height), visible(visible) { }

    bool operator==(const SimpleSpaceObject &other)
    {
        return crds == other.crds && height == other.height;
    }

    Coordinates crds;
    unsigned int height;
    bool visible;
};

#endif // SIMPLESPACEOBJECT_H
