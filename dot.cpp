#include "dot.h"
#include "QDebug"
#include <QIODevice>
#include <QByteArray>
#include "myerror.h"

Dot::Dot()
{

}

Dot::Dot(QString dotFileName)
{

}

void Dot::setFile(QString file)
{
    dotFile.setFileName(file);
}

bool Dot::parseDependencyTree()
{
    qDebug() << "Parsing the dot file" << dotFile.fileName() << endl;

    if(!dotFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open " << dotFile.fileName() << endl;
        return false;
    }

    QTextStream in(&dotFile);
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
        Component p = dependencyTree.at(i).getParent();
        Component c = dependencyTree.at(i).getChild();

        qDebug() << "[" << i << "]" << " parent: " << p.getName() << "@" << p.getTag();
        qDebug() << "[" << i << "]" << " child: " << c.getName() << "@" << c.getTag();
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

    // Add dependencies for each component
    // TODO: Optimize the algorithm
    for (int i = 0; i < dependencyPyramid.size(); i++)
    {
        for (int j = 0; j < dependencyPyramid[i].size(); j++)
        {
            for (int k = 0; k < dependencyTree.size(); k++)
            {
                if (dependencyTree[k].getParent().getName() == dependencyPyramid[i][j].getName()) {
                    dependencyPyramid[i][j].appendDependency(dependencyTree[k].getChild());
                }
            }
        }
    }
}

QString Dot::updateTag(const QString &tag)
{
    QString newTag;

    // BUILD can't be larger than 65535, so we can use int type here
    int build;
    bool ok;
    build = tag.section('.', 3, 3).toInt(&ok, 10);

    if (ok) {
        build += 1;
    }

    newTag = tag.section('.',0, 2) + "." + QString::number(build);

    return newTag;
}

void Dot::updateComponentTagInUpdateList(Component component)
{
    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        if (component.getName() == componentsToUpdate.at(i).getName()) {
            QString tag = componentsToUpdate.at(i).getTag();
            QString newTag = updateTag(tag);
            componentsToUpdate[i].setTag(newTag);
            qDebug() << "Change tag from " << tag << " to " << newTag << " for " << component.getName();
        }
    }
}

void Dot::displayDependencyPyramid() const
{
    for (int level = 0; level < dependencyPyramid.size(); level++)
    {
        qDebug() << "Level " << level << " :";

        for (int pos = 0; pos < dependencyPyramid.at(level).size(); pos++)
        {
            qDebug() << dependencyPyramid.at(level).at(pos).getName();
            dependencyPyramid.at(level).at(pos).displayDependencies();
        }
    }
}

void Dot::generateDependencyPyramidLevel0()
{
    Component c(dependencyTree.at(0).getParent());
    ComponentsList l;

    l << c;
    dependencyPyramid << l;
}

void Dot::generateAllComponentsList()
{
    for (int i = 0; i < dependencyPyramid.size(); i++)
    {
        allComponentsList.append(dependencyPyramid[i]);
    }
}

ComponentsList Dot::getAllComponentsList()
{
    return allComponentsList;
}

void Dot::setComponentToUpdate(Component componentToUpdate)
{
    componentsToUpdate.append(componentToUpdate);
}

void Dot::displayComponentsToUpdate() const
{
    qDebug() << "Components to update are:";

    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        qDebug() << componentsToUpdate.at(i).getName() << componentsToUpdate.at(i).getTag();
    }
}

void Dot::updateLocalManifests()
{
    for (int i = dependencyPyramid.size() - 1; i >= 0; i--)
    {
        ComponentsList tmpComponentsList;

        for (int j = 0; j < dependencyPyramid[i].size(); j++)
        {
            processSingleComponent(dependencyPyramid[i][j], tmpComponentsList);
        }

        componentsToUpdate += tmpComponentsList;
    }
}

void Dot::processSingleComponent(Component componentToProcess, ComponentsList &componentsListNewAdded)
{
    Component component = componentToProcess;
    // componentSpecified doesn't have dependencies !!!
    Component componentSpecified = componentSpecifiedTo(component);

    // alreadySpecified means this component has been specified to be updated by user
    bool alreadySpecified = (componentSpecified.getTag() != component.getTag());

    if (alreadySpecified) {
        componentSpecified.checkoutToTag();

        // TODO: check if the manifests are same
    }

    bool needUpdate;

    // It's not allowed to use the tag info in this method because we pass component.
    // TODO: remove the restrict
    needUpdate = updateSingleManifestIfNeeded(component);

    if (needUpdate) {
        if (alreadySpecified) {
            // This component has been specified by user to be updated, however, as its dependency also needs to be updated,
            // update the tag of this component in the updating component list.
            updateComponentTagInUpdateList(componentSpecified);
        } else {
            component.updateTag();  // Update my tag here
            componentsListNewAdded << component;
        }
    }

}

// It's not allowed to use the tag info in this method !!!
bool Dot::updateSingleManifestIfNeeded(Component component)
{
    bool needUpdate = false;

    ComponentsList dependencies = component.getDependencies();

    for (int i = 0; i < dependencies.size(); i++)
    {
        Component c = componentSpecifiedTo(dependencies[i]);

        // If the tag of this component isn't same with that of this component in updated list
        if (dependencies[i].getTag() != c.getTag()) {
            component.updateDependencyInManifest(dependencies[i], c);
            needUpdate = true;
        }
    }

    if (needUpdate) {
        component.updateBuildInManifest();
        component.commitChangeOfManifest();
        component.creatNewTag();
    }

    return needUpdate;
}

Component Dot::componentSpecifiedTo(Component componentToCheck)
{
    Component c = componentToCheck;

    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        if (c.getName() == componentsToUpdate[i].getName()) {
            qDebug() << c.getName() << " needs to be updated";
            return componentsToUpdate[i];
        }
    }

    return c;
}

void Dot::processLineOfDependencyTree(QString line)
{
    if (line.contains("->")) {
        line.remove(QChar(';'));
        line.remove(QChar('"'));

        DependencyPair pair;

        QString parent = line.section(' ', 0, 0);
        QString child = line.section(' ', 2, 2);

        pair.setParent(parent.section('@', 0, 0), parent.section('@', 1, 1));
        pair.setChild(child.section('@', 0, 0), child.section('@', 1, 1));

        dependencyTree << pair;
    }
}

void Dot::insertPairChildToPyramidLevel(int pairIndex, int level)
{
    DependencyPair p = dependencyTree[pairIndex];
    Component c(p.getChild());

    // If we want to add a component to level N, it means the total levels should be at least N+1
    // There should be only two situations:
    // 1. size = level + 1, we can append c into it directly
    // 2. size = level, we haven't got a avalible list for current c, we need creat one for it.
    int currentMaxLevel = dependencyPyramid.size() - 1;

    if (currentMaxLevel == level - 1) {
        ComponentsList l;
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
    Component parent = dependencyTree[pairIndex].getParent();

    int i;
    for (i = 0; i < list.size(); i++)
    {
        if(parent.is(list.at(i)))
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
    Component child = dependencyTree[pairIndex].getChild();

    int i;
    for (i = 0; i < list.size(); i++)
    {
        if (child.is(list.at(i)))
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

void DependencyPair::setParent(QString name, QString tag)
{
    parent.setName(name);
    parent.setTag(tag);
}

void DependencyPair::setChild(QString name, QString tag)
{
    child.setName(name);
    child.setTag(tag);
}

Component DependencyPair::getParent() const
{
    return parent;
}

Component DependencyPair::getChild() const
{
    return child;
}

