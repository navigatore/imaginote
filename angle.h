#ifndef ANGLE_H
#define ANGLE_H

#include <cmath>
#include <string>

class Angle
{
public:
    explicit Angle(float angle = 0.0f) : value(mod360(angle)) { }

    Angle operator+(Angle other)
    {
        mod360(value + other.value);
        return Angle(value + other.value);
    }

    Angle operator-(const Angle& other)
    {
        return *this + (-other);
    }

    Angle operator/(float x) const
    {
        return Angle(this->value / x);
    }

    Angle& operator+=(float angle)
    {
        value = mod360(value + angle);
        return *this;
    }

    Angle& operator-=(float angle)
    {
        return operator+=(-angle);
    }

    Angle operator-() const
    {
        return Angle(-value);
    }

    static float mod360(float value)
    {
        return std::fmod(std::fmod(value, 360.0f) + 360.0f, 360.0f);
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
