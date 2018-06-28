#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component
{
public:
    Component();
    Component(QString nameOfComponent);
    ~Component();

    void setName(QString nameToBe);
    void setTag(QString tagToBe);
    QString getName() const;
    QString getTag() const;
    virtual bool is(Component anotherComponent);

private:
    QString name;
    QString tag;
};

typedef QList<Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

#endif // COMPONENT_H
