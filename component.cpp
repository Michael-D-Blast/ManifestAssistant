#include "component.h"

Component::Component()
{

}

Component::Component(QString nameOfComponent, int levelOfComponent)
{
    name = nameOfComponent;
    level = levelOfComponent;
}

Component::~Component()
{

}

QString Component::getName() const
{
    return name;
}

int Component::getLevel() const
{
    return level;
}

bool Component::isInLevel(int whichLevel) const
{
    return(level ==  whichLevel);
}
