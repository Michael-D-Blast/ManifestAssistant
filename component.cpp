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

void Component::setName(QString nameToBe)
{
    name = nameToBe;
}

void Component::setTag(QString tagToBe)
{
    tag = tagToBe;
}

QString Component::getName() const
{
    return name;
}

QString Component::getTag() const
{
    return tag;
}

bool Component::is(Component anotherComponent)
{
    return(name == anotherComponent.getName());
}

