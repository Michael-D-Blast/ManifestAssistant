#ifndef DOT_H
#define DOT_H

#include <QList>
#include <QFile>
#include <QByteArray>
#include "component.h"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include "repoenv.h"
#include <QStringList>


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

class Dot : public QObject
{
    Q_OBJECT

public:
    // TODO: Find a better way to remove these variables
    QString componentNeedsBranch;   // We use it to give component name to branch dialog
    QString branchInputInDialog;    // We use it to give branch got from dialog to backend thread

    QString workingDir;     // The working dir

    Dot();

    // Get Methods
    void setFile(QString file);
    bool parseDependencyTree();
    void displayDependencyTree();
    void displayDependencyPyramid() const;  // Display the components and their dependencies in each level
    ComponentsList getAllComponentsList();
    void displayComponentsToUpdate() const;
    void displayPackagesWithoutSourceCode() const;

    // Set Methods
    void setRepoEnv(RepoEnv *repoEnv);
    void generateDependencyPyramidLevel0();
    void generateFirstComponent();
    void generateAllComponentsList();   // Transfer dependencyPyramid to allComponentsList
    void generateDependencyPyramid();   // Generate dependency pyramid
    int fetchSourceCodesOfPackages();   // Download source codes of packages if they have
    void setDependenciesForPyramid();    // Look for dependencies for each component
    void updateLocalManifests();
    int pushLocalCommits();     // Push local commits to remote
//    int makePackages();      // Create package
    static QString updateTag(const QString &tag);
    virtual void setComponentToUpdate(Component componentToUpdate);    // Add a component into the component list to be udpated
    int generatePackagesWithoutSourceCode();

signals:
    void requestBranchDialog();

private:
    QFile dotFile;
    QList<DependencyPair> dependencyTree;   // Pair information read from .dot file
    ComponentsMesh dependencyPyramid;       // Component pyramid from componentsGroup
    ComponentsList componentsToUpdate;
    ComponentsList allComponentsList;   // used for items in combox
    QString rootComponent;      // The entry component of the product, default is Esmeralda
    RepoEnv *repoEnv;           // makePackages needs this information to know if a component is package
    QStringList packagesWithoutSourceCode;

    void processLineOfDependencyTree(QString line);
    void insertPairChildToPyramidLevel(int pairIndex, int level);
    int getParentPosInLevel(int pairIndex, int level);
    int getChildPosInLevel(int pairIndex, int level);
    void removeChildIfInUpperLevel(int pairIndex, int level);
    // We need to change the componentsListNewAdded
    void processSingleComponent(Component component, ComponentsList &componentsListNewAdded);
    // Check if the component's manifest needs to be udpated, if yes, update it and return true, otherwise, return false.
    Component updateSingleManifestIfNeeded(Component component);
    // Check if component is in componentsToUpdate, if yes, return new component infomation, otherwise, return itself
    Component componentSpecifiedTo(Component component);
    // Set Methods
    void updateComponentTagInUpdateList(Component component);
    void removeOldComponentInUpdateList(QString componentName);     // remove the component from the update list
    int makeSinglePackage(Component component);
    ComponentsList getComponentDependenciesFromDependencyTree(QString component);      // Iterate the dependency tree, get the dependencies for the component which is a package and hasn't source code
};

#endif // DOT_H
