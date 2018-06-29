#include "component.h"
#include <QDebug>

Component::Component()
{

}

Component::Component(QString nameOfComponent)
{
    name = nameOfComponent;

}

Component::Component(QString nameOfComponent, QString tagOfComponent)
{
    name = nameOfComponent;
    tag = tagOfComponent;

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

int Component::checkoutToTag()
{
    qDebug() << "Dummy: checkout " << name << " to " << tag;
}

void Component::appendDependency(Component dependentComponent)
{
    dependencies << dependentComponent;
}

void Component::displayDependencies() const
{
    for (int i = 0; i < dependencies.size(); i++)
    {
        qDebug() << "\t" << dependencies.at(i).getName() << "@" << dependencies.at(i).getTag();
    }
}

