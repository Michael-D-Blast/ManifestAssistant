#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component
{
public:
    Component();
    Component(QString nameOfComponent, int levelOfComponent);
    ~Component();

    QString getName() const;
    int getLevel() const;
    bool isInLevel(int whichLevel) const;

private:
    QString name;
    int level;
};

typedef QList<Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

#endif // COMPONENT_H
