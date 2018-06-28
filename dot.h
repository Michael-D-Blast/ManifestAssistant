#ifndef DOT_H
#define DOT_H

#include <QList>
#include <QFile>
#include <QByteArray>


class Component {
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

class DependencyPair {
public:
    DependencyPair();
    ~DependencyPair();

    void setParent(QString parentInDependencyPair);
    void setChild(QString childInDependencyPair);
    QString getParent() const;
    QString getChild() const;
    bool isRelationOfParent(class Component component) const;

private:
    QString parent;
    QString child;

};

class Dot
{
public:
    Dot();
    Dot(QString dotFileName);   // absolete, use setFile

    void setFile(QString file);
    bool parseDependencyTree();
    void displayDependencyTree();

    void generateDependencyPyramid();
    void displayDependencyPyramid() const;
    void generateDependencyPyramidLevel0();
    void generateFirstComponent();

private:
    QFile dotFile;
    // Pair information read from .dot file
    QList<DependencyPair> dependencyTree;
    // Component pyramid from componentsGroup
    ComponentsMesh dependencyPyramid;
    ComponentsList componentsToUpdate;


    void processLineOfDependencyTree(QString line);
    void insertPairChildToPyramidLevel(int pairIndex, int level);
    int getParentPosInLevel(int pairIndex, int level);
    int getChildPosInLevel(int pairIndex, int level);
    void removeChildIfInUpperLevel(int pairIndex, int level);
};

#endif // DOT_H
