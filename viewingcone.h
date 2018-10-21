#ifndef VIEWINGCONE_H
#define VIEWINGCONE_H

#include "angle.h"

class ViewingCone
{
public:
    ViewingCone(Angle viewAngleX, Angle viewAngleY, float maxDistance) : viewAngleX(viewAngleX), viewAngleY(viewAngleY), maxDistance(maxDistance) { }

    Angle viewAngleX, viewAngleY;
    float maxDistance;
};

#endif // VIEWINGCONE_H
