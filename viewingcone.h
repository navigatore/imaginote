#ifndef VIEWINGCONE_H
#define VIEWINGCONE_H

#include "angle.h"
#include "coordinates.h"

class ViewingCone
{
public:
    ViewingCone(Angle direction, Angle viewAngleX, Angle viewAngleY, float maxDistance) :
        direction(direction),
        viewAngleX(viewAngleX),
        viewAngleY(viewAngleY),
        maxDistance(maxDistance),
        moveSpeed(1.0f),
        rotateSpeed(45.0f)
    { }

    void setPosition(Coordinates position) { this->position = position; }
    Coordinates getPosition() { return position; }

    Angle getDirection() { return direction; }

    void rotateLeft(float time) { direction += rotateSpeed * time; }
    void rotateRight(float time) { direction -= rotateSpeed * time; }
    void forward(float time);
    void backward(float time);

private:
    Coordinates position;
    Angle direction, viewAngleX, viewAngleY;
    float maxDistance, moveSpeed, rotateSpeed;
};

#endif // VIEWINGCONE_H
