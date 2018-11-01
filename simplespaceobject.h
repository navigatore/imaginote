#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include "coordinates.h"


class SimpleSpaceObject
{
public:
    SimpleSpaceObject(Coordinates crd,
                      unsigned int height,
                      bool visible = false,
                      bool focus = false)
        : crds(crd), height(height), visible(visible), focus(focus) { }

    bool operator==(const SimpleSpaceObject &other)
    {
        return crds == other.crds && height == other.height;
    }

    Coordinates crds;
    unsigned int height;
    bool visible, focus;
};

#endif // SIMPLESPACEOBJECT_H
