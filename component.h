#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QList>
static const QString TMP_COMPONENT_DIR = "/tmp/components";

typedef QList<class Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

class Component
{
public:
    Component();
    Component(QString nameOfComponent);
    Component(QString nameOfComponent, QString tagOfComponent);
    ~Component();

    // Get Methods
    QString getName() const;
    QString getTag() const;
    ComponentsList getDependencies();
    virtual bool is(Component anotherComponent);
    void displayDependencies() const;

    // Set Methods
    void setName(QString nameToBe);

    void setTag(QString tagToBe);

    int checkoutToTag();

    void appendDependency(Component dependentComponent);

    int updateDependencyInManifest(Component oldDependency, Component newDependency);

    // Return new BUILD in QString
    QString updateBuildInManifest();

    int commitChangeOfManifest();

    int creatNewTag();

    void updateTag(void);

private:
    QString name;
    QString tag;

    ComponentsList dependencies;
};


#endif // COMPONENT_H
