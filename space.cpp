#include "space.h"
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
                initListenerPos = Coordinates(x, 0, z);
            }
            else
            {
                height = static_cast<unsigned int>(std::stoi(tmp));
            }
            fields[z].push_back(SimpleSpaceObject(Coordinates(x, 0, z), height, height > 0));
        }
    }

    // TODO: Check, if EOF

    f.close();
}
//*********************************************************************************************************************
void Space::printVisibleObjects()
{
    for (auto row : fields)
    {
        for (auto object : row)
        {
            if (object.visible)
            {
                std::cout << object.height << " ";
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
