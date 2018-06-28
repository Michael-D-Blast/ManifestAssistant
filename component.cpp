#include "component.h"

Component::Component()
{

}

Component::Component(QString nameOfComponent)
{
    name = nameOfComponent;
}

Component::~Component()
{

}

QString Component::getName() const
{
    return name;
}

