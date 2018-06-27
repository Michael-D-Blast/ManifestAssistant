#include "dot.h"
#include "QDebug"
#include <QIODevice>
#include <QByteArray>

Dot::Dot()
{

}

Dot::Dot(QString dotFileName)
{
    dotFile = new QFile(dotFileName);
}

bool Dot::parseDependencyTree()
{
    qDebug() << "Parsing the dot file" << dotFile->fileName() << endl;

    if(!dotFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open " << dotFile->fileName() << endl;
        return false;
    }

    QTextStream in(dotFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        processLineOfDependencyTree(line);
    }

    displayDependencyTree();

    return true;
}

void Dot::displayDependencyTree()
{
    for (int i = 0; i < dependencyTree.size(); i++)
    {
        qDebug() << "[" << i << "]" << " parent: " << dependencyTree.at(i).getParent();
        qDebug() << "[" << i << "]" << " child: " << dependencyTree.at(i).getChild();
    }
}

void Dot::generateDependencyPyramid()
{
    generateDependencyPyramidLevel0();

    int currentPair = 0;
    int currentLevel = 1;
    int posInLevel;

    for (currentPair = 0; currentPair < dependencyTree.size(); currentPair++)
    {
        DependencyPair pair = dependencyTree[currentPair];
//        if (!levelNIncludesPairParent(currentLevel - 1, pair)) {
        if ((posInLevel = getParentPosInLevel(currentPair, currentLevel - 1)) < 0) {
            currentLevel++;
            QList<Component> list;
            dependencyPyramid << list;
        }

        if ((posInLevel = getChildPosInLevel(currentPair, currentLevel)) < 0) {
            // Insert current pair's child into current level list
            insertPairChildToPyramidLevel(currentPair, currentLevel);
        }

        removeChildIfInUpperLevel(currentPair, currentLevel);
    }
}

void Dot::displayDependencyPyramid() const
{
    for (int level = 0; level < dependencyPyramid.size(); level++)
    {
        qDebug() << "Level " << level << " :" << endl;

        for (int pos = 0; pos < dependencyPyramid.at(level).size(); pos++)
        {
            qDebug() << dependencyPyramid.at(level).at(pos).getName() << "\t";
        }
    }
}

void Dot::generateDependencyPyramidLevel0()
{
    class Component c(dependencyTree.at(0).getParent(), 0);
    QList<class Component> l;

    l << c;
    dependencyPyramid << l;
}

void Dot::generateFirstComponent()
{
    class Component c(dependencyTree.at(0).getParent(), 0);
    componentsGroup << c;
}

bool Dot::componentsGroupIncludes(Component c)
{
    for (int i = 0; i < componentsGroup.size(); i++)
    {
        if (c.getName() == componentsGroup.at(i).getName()) {
            return true;
        }
    }

    return false;
}

void Dot::componentsGroupChangeComponentLevelIfDiff(Component c)
{
    for (int i = 0; i < componentsGroup.size(); i++)
    {
        if (c.getName() == componentsGroup.at(i).getName()) {
            if (c.getLevel() != componentsGroup.at(i).getLevel())
                componentsGroup.replace(i, c);
        }
    }
}

void Dot::displayComponentsGroup() const
{
    for (int i = 0; i < componentsGroup.size(); i++)
    {
        qDebug() << componentsGroup.at(i).getName() << " in level " << componentsGroup.at(i).getLevel() << endl;
    }
}

void Dot::processLineOfDependencyTree(QString line)
{
    if (line.contains("->")) {
        line.remove(QChar(';'));
        class DependencyPair pair;
        pair.setParent(line.section(' ', 0, 0));
        pair.setChild(line.section(' ', 2, 2));
        dependencyTree << pair;
    }
}

void Dot::insertPairChildToPyramidLevel(int pairIndex, int level)
{
    class DependencyPair p = dependencyTree[pairIndex];
    class Component c(p.getChild(), level);

    // If we want to add a component to level N, it means the total levels should be at least N+1
    // There should be only two situations:
    // 1. size = level + 1, we can append c into it directly
    // 2. size = level, we haven't got a avalible list for current c, we need creat one for it.
    int currentMaxLevel = dependencyPyramid.size() - 1;

    if (currentMaxLevel == level - 1) {
        QList<class Component> l;
        dependencyPyramid << l;
        dependencyPyramid[level] << c;
    } else if (currentMaxLevel == level) {
        dependencyPyramid[level] << c;
    } else {
        qCritical() << "currentMaxLevel is " << currentMaxLevel << " level is " << level << endl;
    }

}

int Dot::getParentPosInLevel(int pairIndex, int level)
{
    if (dependencyPyramid.size() -1 < level) {
        return -1;
    }

    QList<Component> list = dependencyPyramid[level];
    QString parent = dependencyTree[pairIndex].getParent();

    int i;
    for (i = 0; i < list.size(); i++)
    {
        if (parent == list.at(i).getName())
            return i;
    }

    return -1;
}

int Dot::getChildPosInLevel(int pairIndex, int level)
{
    // If we don't have level N (which is size - 1), then we don't need to find it in this level.
    if (dependencyPyramid.size() -1 < level) {
        return -1;
    }

    QList<Component> list = dependencyPyramid[level];
    QString child = dependencyTree[pairIndex].getChild();

    int i;
    for (i = 0; i < list.size(); i++)
    {
        if (child == list.at(i).getName())
            return i;
    }

    return -1;
}

void Dot::removeChildIfInUpperLevel(int pairIndex, int level)
{
    int pos;

    // We don't need to find it in level 0, because level zero is root component. So i > 0.
    for (int i = level - 1; i > 0; i--) {
        if ((pos = getChildPosInLevel(pairIndex, i)) >= 0) {
            dependencyPyramid[i].removeAt(pos);
        }
    }
}

DependencyPair::DependencyPair()
{

}

DependencyPair::~DependencyPair()
{

}

void DependencyPair::setParent(QString parentInDependencyPair)
{
    parent = parentInDependencyPair;
}

void DependencyPair::setChild(QString childInDependencyPair)
{
    child = childInDependencyPair;
}

QString DependencyPair::getParent() const
{
    return parent;
}

QString DependencyPair::getChild() const
{
    return child;
}

bool DependencyPair::isRelationOfParent(Component component) const
{
    return (parent == component.getName());
}




Component::Component()
{

}

Component::Component(QString nameOfComponent, int levelOfComponent)
{
    name = nameOfComponent;
    level = levelOfComponent;
}

Component::~Component()
{

}

QString Component::getName() const
{
    return name;
}

int Component::getLevel() const
{
    return level;
}

bool Component::isInLevel(int whichLevel) const
{
    return(level ==  whichLevel);
}



