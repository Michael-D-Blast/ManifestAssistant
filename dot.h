#ifndef DOT_H
#define DOT_H

#include <QList>
#include <QFile>
#include <QByteArray>
#include "component.h"


class DependencyPair {
public:
    DependencyPair();
    ~DependencyPair();

    void setParent(QString name, QString tag);
    void setChild(QString name, QString tag);
    Component getParent() const;
    Component getChild() const;

private:
    Component parent;
    Component child;

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
    // Transfer dependencyPyramid to allComponentsList
    void generateAllComponentsList();
    ComponentsList getAllComponentsList();
    virtual void setComponentToUpdate(Component componentToUpdate);
    void displayComponentsToUpdate() const;
    void updateLocalManifests();

private:
    QFile dotFile;
    // Pair information read from .dot file
    QList<DependencyPair> dependencyTree;
    // Component pyramid from componentsGroup
    ComponentsMesh dependencyPyramid;
    ComponentsList componentsToUpdate;
    ComponentsList allComponentsList;   // used for items in combox


    void processLineOfDependencyTree(QString line);
    void insertPairChildToPyramidLevel(int pairIndex, int level);
    int getParentPosInLevel(int pairIndex, int level);
    int getChildPosInLevel(int pairIndex, int level);
    void removeChildIfInUpperLevel(int pairIndex, int level);
    void processSingleComponent(Component componentToProcess, ComponentsList componentsListNewAdded);
    // Check if the component's manifest needs to be udpated, if yes, update it and return true, otherwise, return false.
    bool updateSingleManifestIfNeeded(Component component);
    // Check if component is in componentsToUpdate, if yes, return new component infomation, otherwise, return itself
    Component componentSpecifiedTo(Component componentToCheck);
};

#endif // DOT_H
