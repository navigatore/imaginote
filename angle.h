#ifndef ANGLE_H
#define ANGLE_H

#include <cmath>
#include <string>

class Angle
{
public:
    explicit Angle(float angle = 0.0f) : value(std::fmod(angle, 360.0f)) { }

    Angle& operator+=(float angle)
    {
        value = std::fmod(std::fmod(value + angle, 360.0f) + 360.0f, 360.0f);
        return *this;
    }

    Angle& operator-=(float angle)
    {
        return operator+=(-angle);
    }

    float getRad()
    {
        return value * 3.1415f / 180.0f;
    }

    std::string str()
    {
        return std::to_string(value) + " deg";
    }

    float value;
};

#endif // ANGLE_H
