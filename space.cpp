#include "space.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
//*********************************************************************************************************************
Space::Space(int updateFreq)
 : updateFreq(updateFreq),
   cone(Angle(90.0f), Angle(180.0f), Angle(180.0f), 1000.0f),
   focusAngleVal(0.0f), focusAngleMoveSpeed(30.0f),
   closestField(0.0f, 0.0f, 0.0f),
   sp(nullptr),
   closestFieldExists(false),
   closestFieldChanged(false),
   movingFocusAngle(false),
   mapWidget(nullptr)
{ }
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
void Space::rotateListenerLeft(float time)
{
    cone.rotateLeft(time);
}
//*********************************************************************************************************************
void Space::rotateListenerRight(float time)
{
    cone.rotateRight(time);
}
//*********************************************************************************************************************
void Space::goForward(float time)
{
    cone.forward(time);
}
//*********************************************************************************************************************
void Space::goBackward(float time)
{
    cone.backward(time);
}
//*********************************************************************************************************************
void Space::printVisibleObjects()
{
    auto roundedPos = cone.getPosition();
    roundedPos.x = std::round(roundedPos.x);
    roundedPos.y = std::round(roundedPos.y);
    roundedPos.z = std::round(roundedPos.z);

    for (auto row : fields)
    {
        for (auto field : row)
        {
            if (field.crds == roundedPos)
            {
                std::cout << "x ";
            }
            else if (field == closestField)
            {
                std::cout << "* ";
            }
            else if (field.focus)
            {
                std::cout << "F ";
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
void Space::startPlaying()
{
    mapWidget->loadMap(fields);
    mapWidget->setAngleX(cone.getAngleX() / 2);
    sp = new NewSpacePlayer();
    sp->sonificateObject(closestField);
}
//*********************************************************************************************************************
void Space::stopPlaying()
{
    delete sp;
    sp = nullptr;
    mapWidget->unloadMap();
}
//*********************************************************************************************************************
void Space::update()
{
    if (movingFocusAngle)
    {
        moveFocusAngle();
    }
    setFieldsVisibility();
    setFieldsFocus();
    updateClosestFocusField();
    auto closestFieldPtr = closestFieldExists ? &closestField : nullptr;
    mapWidget->update(
                cone.getPosition(),
                cone.getDirection(),
                cone.getDirection() + Angle(focusAngleVal),
                closestFieldPtr
    );
    playClosestFocusField();
    sp->updateListenerPosition(cone.getPosition(), cone.getDirection());
}
//*********************************************************************************************************************
void Space::toggleMovingFocusAngle()
{
    movingFocusAngle = !movingFocusAngle;
}
//*********************************************************************************************************************
void Space::setAngleX(const Angle &angleX)
{
    cone.setAngleX(angleX);
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
void Space::setFieldsFocus()
{
    for (auto &row : fields)
    {
        for (auto &field : row)
        {
            if (field.visible && lookingAt(field))
            {
                field.focus = true;
            }
            else
            {
                field.focus = false;
            }
        }
    }
}
//*********************************************************************************************************************
void Space::updateClosestFocusField()
{
    auto oldClosest = closestField;
    auto existed = closestFieldExists;
    closestField = SimpleSpaceObject(0.0f, 0.0f, 0.0f);
    closestFieldExists = false;

    for (auto row : fields)
    {
        for (auto field : row)
        {
            if (!closestFieldExists)
            {
                if (field.focus)
                {
                    closestField = field;
                    closestFieldExists = true;
                }
            }
            else
            {
                if (field.focus && firstCloser(field, closestField))
                {
                    closestField = field;
                }
            }
        }
    }
    closestFieldChanged = ((closestFieldExists && !existed) || (!closestFieldExists && existed) || (oldClosest != closestField));
}
//*********************************************************************************************************************
void Space::playClosestFocusField()
{
    if (sp != nullptr && closestFieldChanged)
    {
        if (closestFieldExists)
        {
            sp->sonificateObject(closestField);
        }
        else
        {
            sp->stopPlaying();
        }
    }
}
//*********************************************************************************************************************
bool Space::lookingAt(const SimpleSpaceObject &object)
{
    auto focusAngle = Angle(focusAngleVal);

    Coordinates tl(object.crds), tr(object.crds), bl(object.crds), br(object.crds);
    tl.x -= 0.5f;
    tl.z -= 0.5f;
    tr.x += 0.5f;
    tr.z -= 0.5f;
    bl.x -= 0.5f;
    bl.z += 0.5f;
    br.x += 0.5f;
    br.z += 0.5f;

    bool allLeft = cone.onLeftSide(focusAngle, tl)
            && cone.onLeftSide(focusAngle, tr)
            && cone.onLeftSide(focusAngle, bl)
            && cone.onLeftSide(focusAngle, br);
    bool allRight = !cone.onLeftSide(focusAngle, tl)
            && !cone.onLeftSide(focusAngle, tr)
            && !cone.onLeftSide(focusAngle, bl)
            && !cone.onLeftSide(focusAngle, br);

    return !allLeft && !allRight;
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
void Space::moveFocusAngle()
{
    focusAngleVal += focusAngleMoveSpeed / updateFreq;
    if (focusAngleVal > cone.getAngleX().value / 2.0f)
        focusAngleVal = -cone.getAngleX().value / 2.0f;
}
//*********************************************************************************************************************
