#include "spaceobject.h"
#include <cmath>

float SpaceObject::getDistanceFromCenter()
{
    return distance3d(getCenter(), Coordinates(0, 0, 0));
}

float SpaceObject::getWHRatio()
{
    auto width = distance3d(lb, rb);
    auto height = distance3d(lb, lt);
    return width / height;
}

float SpaceObject::distance3d(Coordinates a, Coordinates b)
{
    float diff_x = b.x - a.x, diff_y = b.y - a.y, diff_z = b.z - a.z;
    float dist_xy = std::sqrt(diff_x * diff_x + diff_y * diff_y);
    float dist_xyz = std::sqrt(diff_z * diff_z + dist_xy * dist_xy);
    return dist_xyz;
}
