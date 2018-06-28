#include "component.h"
#include <QDebug>

Component::Component()
{
    dependencies = new QList<QString>;
}

Component::Component(QString nameOfComponent)
{
    name = nameOfComponent;
    dependencies = new QList<QString>;
}

Component::Component(QString nameOfComponent, QString tagOfComponent)
{
    name = nameOfComponent;
    tag = tagOfComponent;
    dependencies = new QList<QString>;
}

Component::~Component()
{
    delete dependencies;
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

void Component::appendDependency(const QString &dependentComponent)
{
    *dependencies << dependentComponent;
}

