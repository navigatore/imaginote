#ifndef COORDINATES_H
#define COORDINATES_H

#include <string>

class Coordinates
{
public:
    Coordinates(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) { }

    std::string str()
    {
        return "X: " + std::to_string(x) + "  Y: " + std::to_string(y) + "  Z: " + std::to_string(z);
    }

    bool operator==(const Coordinates &other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    float x, y, z;
};

#endif // COORDINATES_H
