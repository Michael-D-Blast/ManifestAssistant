#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QList>

typedef QList<class Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

class Component
{
public:
    Component();
    Component(QString nameOfComponent);
    Component(QString nameOfComponent, QString tagOfComponent);
    ~Component();

    void setName(QString nameToBe);
    void setTag(QString tagToBe);
    QString getName() const;
    QString getTag() const;
    virtual bool is(Component anotherComponent);

    int checkoutToTag();
    void appendDependency(const QString &dependentComponent);

private:
    QString name;
    QString tag;

    QList<QString> *dependencies;

};


#endif // COMPONENT_H
