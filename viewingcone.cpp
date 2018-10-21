#include "viewingcone.h"
//*********************************************************************************************************************
void ViewingCone::forward(float time)
{
    position.x += std::cos(direction.getRad()) * time;
    position.z -= std::sin(direction.getRad()) * time;
}
//*********************************************************************************************************************
void ViewingCone::backward(float time)
{
    position.x -= std::cos(direction.getRad()) * time;
    position.z += std::sin(direction.getRad()) * time;
}
//*********************************************************************************************************************
