#include "space.h"
#include <exception>
#include <fstream>

void Space::loadFromFile(const char *fname)
{
    fields.clear();

    std::ifstream f;
    f.open(fname);

    std::string spaceName;
    unsigned int height, width;

    f >> spaceName >> height >> width;

    if (!f)
        throw InvalidSpaceFile();

    for (unsigned int y = 0; y < width; ++y)
    {
        fields.push_back(std::vector<int>());
        for (unsigned int x = 0; x < height; ++x)
        {
            std::string tmp;
            f >> tmp;
            if (tmp == "x")
            {
                initListenerPos = Coordinates(x, y);
                fields[y].push_back(0);
            }
            else
            {
                fields[y].push_back(std::stoi(tmp));
            }
        }
    }

    int i;
    i = 5;

    // TODO: Check, if EOF
}
