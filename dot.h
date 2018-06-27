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
    Dot(QString dotFileName);

    bool parseDependencyTree();
    void displayDependencyTree();

    void generateDependencyPyramid();
    void displayDependencyPyramid() const;
    void generateDependencyPyramidLevel0();
    void generateFirstComponent();
    bool componentsGroupIncludes(class Component c);
    void componentsGroupChangeComponentLevelIfDiff(class Component c);
    void displayComponentsGroup() const;

private:
    QFile *dotFile;
    // Pair information read from .dot file
    QList<class DependencyPair> dependencyTree;
    // Components information from dependencyTree
    QList<class Component> componentsGroup;
    // Component pyramid from componentsGroup
    QList<QList<class Component>> dependencyPyramid;


    void processLineOfDependencyTree(QString line);
    void insertPairChildToPyramidLevel(int pairIndex, int level);
    int getParentPosInLevel(int pairIndex, int level);
    int getChildPosInLevel(int pairIndex, int level);
    void removeChildIfInUpperLevel(int pairIndex, int level);
};

#endif // DOT_H
