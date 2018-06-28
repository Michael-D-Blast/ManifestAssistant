#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component
{
public:
    Component();
    Component(QString nameOfComponent);
    ~Component();

    QString getName() const;
    QString getTag() const;

private:
    QString name;
    QString tag;
};

typedef QList<Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

#endif // COMPONENT_H
