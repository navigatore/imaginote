#include "space.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

//*********************************************************************************************************************
void Space::loadFromFile(const char *fname)
{
    fields.clear();

    std::ifstream f;
    f.open(fname);

    unsigned int height, width;

    f >> name >> height >> width;

    if (!f)
        throw InvalidSpaceFile();

    for (unsigned int z = 0; z < width; ++z)
    {
        fields.push_back(std::vector<SimpleSpaceObject>());
        for (unsigned int x = 0; x < height; ++x)
        {
            unsigned int height = 0;
            std::string tmp;
            f >> tmp;
            if (tmp == "x")
            {
                cone.setPosition(Coordinates(x, 0, z));
            }
            else
            {
                height = static_cast<unsigned int>(std::stoi(tmp));
            }
            auto obj = SimpleSpaceObject(Coordinates(x, 0, z), height, height > 0);
            fields[z].push_back(obj);
        }
    }

    // TODO: Check, if EOF

    f.close();

    update();
}
//*********************************************************************************************************************
void Space::printVisibleObjects()
{
    for (auto row : fields)
    {
        for (auto field : row)
        {
            if (field == closestField)
            {
                std::cout << "* ";
            }
            else if (field.visible)
            {
                std::cout << field.height << " ";
            }
            else
            {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
//*********************************************************************************************************************
void Space::update()
{
    setFieldsVisibility();
    findClosestField();
}
//*********************************************************************************************************************
void Space::setFieldsVisibility()
{
    for (auto &row : fields)
    {
        for (auto &field : row)
        {
            if (field.height > 0 && cone.isInside(field.crds))
            {
                field.visible = true;
            }
            else
            {
                field.visible = false;
            }
        }
    }
}
//*********************************************************************************************************************
void Space::findClosestField()
{
    closestFieldExists = false;
    for (auto row : fields)
    {
        for (auto field : row)
        {
            if (!closestFieldExists)
            {
                if (field.visible)
                {
                    closestField = field;
                    closestFieldExists = true;
                }
            }
            else
            {
                if (field.visible && firstCloser(field, closestField))
                {
                    closestField = field;
                }
            }
        }
    }
}
//*********************************************************************************************************************
bool Space::firstCloser(const SimpleSpaceObject &first, const SimpleSpaceObject &second)
{
    return distanceSqFrom(first) < distanceSqFrom(second);
}
//*********************************************************************************************************************
float Space::distanceSqFrom(SimpleSpaceObject obj)
{
    auto x_diff = obj.crds.x - cone.getPosition().x;
    auto z_diff = obj.crds.z - cone.getPosition().z;
    return x_diff * x_diff + z_diff * z_diff;
}
//*********************************************************************************************************************
