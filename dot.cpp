#include "dot.h"
#include "QDebug"
#include <QIODevice>
#include <QByteArray>
#include "myerror.h"
#include "gitexecutor.h"
#include <QStringList>
#include <QMutex>
#include <QWaitCondition>
#include <QMessageBox>
#include <QDir>
#include "cmdexecutor.h"
#include "repoexecutor.h"
#include "componentpkgdir.h"
#include "componentsrcdir.h"
#include "repomanifest.h"

// TODO: Select the workspace dir in a dialog
static const QString WORKSPACE_DIR = "/home/abb/mtws/";
extern QMutex complete;
extern QWaitCondition waitCondition;
static const QString DEFAULT_ROOT_COMPONENT = "Esmeralda";
const QString PACKAGES_WITHOUT_SOURCE_CODE_FILE = "packages_without_source_code";
const QString PACKAGES_SOURCE_CODES_DIR = "/home/abb/packages_source_codes/";

Dot::Dot()
{
    rootComponent = DEFAULT_ROOT_COMPONENT;
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

    dotFile.close();

//    displayDependencyTree();

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
}

int Dot::fetchSourceCodesOfPackages()
{
    int ret = 0;

    QDir dir(PACKAGES_SOURCE_CODES_DIR);

    if (!dir.exists())
        dir.mkpath(PACKAGES_SOURCE_CODES_DIR);

    try
    {
        ComponentSrcDir build(PACKAGES_SOURCE_CODES_DIR, "Build", "sz");
        build.init();

        for (int i = 0; i < dependencyPyramid.size(); i++)
        {
            for (int j = 0; j < dependencyPyramid[i].size(); j++)
            {
                Component dependency = dependencyPyramid[i][j];
                if (dependency.isPackage(repoEnv) && dependency.hasSourceCode(packagesWithoutSourceCode))
                {
                    ComponentSrcDir dependencyDir(PACKAGES_SOURCE_CODES_DIR, dependency.getName(), dependency.getTag());
                    dependencyDir.init();
                }
            }
        }
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}

void Dot::setDependenciesForPyramid()
{
    for (int i = 0; i < dependencyPyramid.size(); i++)
    {
        for (int j = 0; j < dependencyPyramid[i].size(); j++)
        {
            QString component = dependencyPyramid[i][j].getName();
            ComponentsList dependencies;
            GitExecutor git;
            RepoExecutor repo;

            try
            {
                if (repoEnv->isPackage(component))  // If this is a package
                {
                    if (packagesWithoutSourceCode.contains(component))  // and it doesn't have source code
                    {
                        dependencies = getComponentDependenciesFromDependencyTree(component);      // read its dependencies from repo.dot
                    }
                    else    // otherwise, this package has source code
                    {
                        git.checkout(dependencyPyramid[i][j].getTag(), PACKAGES_SOURCE_CODES_DIR + "/" + component);    // git checkout the source code to the tag
                        dependencies = repo.getList(PACKAGES_SOURCE_CODES_DIR + "/" + component);       // call repo list --names-only to get its dependencies
                    }
                }
                else    // If this component is built form source code
                {
                    dependencies = repo.getList(WORKSPACE_DIR + "/" + component);   // call repo list in workspace to get its dependencies
                }
            }
            catch (MyError e)
            {
                e.displayError();
                throw;
            }

            dependencyPyramid[i][j].appendDependency(dependencies);
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

        // TODO: If BUILD is less than 10000, pop a dialog to let user set the tag

        if (build < 10000)      // According to our tag management policy, tag are values larger than 10000
            build = 10000;
        else
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
            QString oldTag = componentsToUpdate.at(i).getTag();
            QString newTag = component.getTag();
            componentsToUpdate[i].setTag(newTag);   // Update the component's tag in updatelist
            componentsToUpdate[i].setUpdated(true);
            qDebug() << "Change tag from " << oldTag << " to " << newTag << " for " << component.getName();
        }
    }
}

void Dot::removeOldComponentInUpdateList(QString componentName)
{
    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        if (componentName == componentsToUpdate.at(i).getName()) {
            componentsToUpdate.removeAt(i);
            qDebug() << "Remove " << componentName << " from componentsToUpdate at position " << i;
            break;
        }
    }
}

int Dot::makeSinglePackage(Component component)
{
    int ret = 0;

    if (component.hasSourceCode(packagesWithoutSourceCode))
    {
        ComponentSrcDir build(TMP_COMPONENT_DIR, "Build", "sz");
        if (!build.exists())
            build.init();

        ComponentSrcDir componentSrcDir(TMP_COMPONENT_DIR, component.getName(), component.getTag());
        componentSrcDir.makePackage();
    }
    else
    {
        ComponentPkgDir componentPkgDir(TMP_COMPONENT_DIR, component.getName(), component.getTag());
        componentPkgDir.makePackage();
    }

    return ret;
}

ComponentsList Dot::getComponentDependenciesFromDependencyTree(QString component)
{
    ComponentsList dependencies;

    QList<DependencyPair>::iterator pair;
    for (pair = dependencyTree.begin(); pair != dependencyTree.end(); ++pair) {
        if (pair->getParent().getName() == component) {
            Component c(pair->getChild().getName(), pair->getChild().getTag());
            dependencies << c;
        }
    }

    return dependencies;
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
    QString currentBranch;

    // Get the branch of Esmeralda
    GitExecutor gitExecutor;
    currentBranch = gitExecutor.getCurrentBranch(workingDir + "/" + rootComponent);
    qDebug() << "Current branch is " << currentBranch;

    c.setTag(currentBranch);    // For the root component, we can't use its tag because it is always master, so we use its current branch as tag

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

int Dot::generatePackagesWithoutSourceCode()
{
    QFile f(PACKAGES_WITHOUT_SOURCE_CODE_FILE);
    if (f.exists()) {
        qDebug() << PACKAGES_WITHOUT_SOURCE_CODE_FILE << " exists";

        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open " << PACKAGES_WITHOUT_SOURCE_CODE_FILE;

            return -1;
        }

        QTextStream in(&f);

        while(!in.atEnd()) {
            QString line = in.readLine();

            if (line.contains('#'))
                continue;

            packagesWithoutSourceCode += line.split(' ', QString::SkipEmptyParts);
        }
    }

    return 0;
}

void Dot::displayComponentsToUpdate() const
{
    qDebug() << "Components to update are:";

    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        Component c = componentsToUpdate.at(i);
        qDebug() << c.getName() << "is going to be updated to " << c.getTag();
    }
}

void Dot::displayPackagesWithoutSourceCode() const
{
    QStringList::const_iterator constIterator;
    for (constIterator = packagesWithoutSourceCode.constBegin(); constIterator != packagesWithoutSourceCode.constEnd();
         ++constIterator)
        qDebug() << (*constIterator) << endl;
}

void Dot::setRepoEnv(RepoEnv *repoEnv)
{
    this->repoEnv = repoEnv;
}

void Dot::updateLocalManifests()
{
    for (int i = dependencyPyramid.size() - 1; i >= 0; i--)
    {
        ComponentsList tmpComponentsList;

        for (int j = 0; j < dependencyPyramid[i].size(); j++)
        {
            try {
                processSingleComponent(dependencyPyramid[i][j], tmpComponentsList);     // Process one component, if it needs to be updated, append it to tmpComponentsList
            }
            catch (MyError e) {
                throw;
            }
        }

        componentsToUpdate += tmpComponentsList;
    }
}

int Dot::pushLocalCommits()
{
    int ret = 0;

    qDebug() << "Dummy: push local commits to remote";
    Component c;

    for (int i = 0; i < componentsToUpdate.size(); i++) {
        c = componentsToUpdate[i];

        // Although a component is in componentsToUpdate, it doesn't mean it needs to be push to remote.
        // It maybe a component specified by user.
        // So We still need to checkout by needToBeUpdated

        if (c.needToBeUpdated()) {
            if (!c.isPackageWithoutSourceCode(repoEnv, packagesWithoutSourceCode))
            {
                qDebug() << "Push local commits of " << c.getName() << " to remote branch " << c.getBranchToCommit();

                GitExecutor gitExecutor;
                try
                {
                    gitExecutor.push(c.getBranchToCommit(), TMP_COMPONENT_DIR + "/" + c.getName());
                }
                catch (MyError e)
                {
                    e.displayError();
                    return -1;
                }
            }

            if (c.isPackage(repoEnv))
            {
                makeSinglePackage(c);
            }
        }
    }

    return ret;
}

//int Dot::makePackages()
//{
//    int ret = 0;

//    qDebug() << "Creating packages ...";
//    Component c;

//    for (int i = 0; i < componentsToUpdate.size(); i++)
//    {
//        c = componentsToUpdate[i];
//        if (c.needToBeUpdated())
//        {
//            if (c.isPackage(repoEnv))
//            {
//                makeSinglePackage(c);
//            }
//        }
//    }

//    return ret;
//}

// Process one component, if it needs to be updated, append it to tmpComponentsList

void Dot::processSingleComponent(Component component, ComponentsList &componentsListNewAdded)
{
    // If the component needs to be updated, return the component with new tag, otherwise, return itself.
    Component componentSpecified = componentSpecifiedTo(component);

    // alreadySpecified means the user wants to update the component
    bool alreadySpecified = (componentSpecified.getTag() != component.getTag());

    if (alreadySpecified)   // If the user wants to update the component
    {
        componentSpecified.checkoutToTag(); // Download the component and checkout to the new tag

        // TODO: check if the manifests are same

        // Generate its commit message file
        componentSpecified.generateCommitMessageFileBetweenTags(component.getTag(), componentSpecified.getTag());   // Record the log between old tag and new tag
    }

    Component finalComponent;

    try {
        finalComponent = updateSingleManifestIfNeeded(componentSpecified);  // If the component's dependency needs to be updated, update manifest, commit, create new tag
    }
    catch (MyError e) {
        e.displayError();
        throw;
    }

    if (finalComponent.needToBeUpdated()) {
        // If this component has already been specified by user, we need to remove it.
        if (alreadySpecified) {
            // This component has been specified by user to be updated, however, as its dependency also needs to be updated,
            // So remove it from the updating component list.
            removeOldComponentInUpdateList(finalComponent.getName());
        }

        componentsListNewAdded << finalComponent;
    }

}

// If the component's dependency needs to be updated, update manifest, commit, create new tag
// If the component is specified by user, it's the one with new tag, otherwise, it's the original component
Component Dot::updateSingleManifestIfNeeded(Component component)
{
    ComponentsList dependencies = component.getDependencies();

    for (int i = 0; i < dependencies.size(); i++)   // Iterate this component's dependencies
    {
        Component currentDependency = dependencies[i];

        // TODO: Don't use componentSpecifiedTo to check if this component needs to be updated

        Component c = componentSpecifiedTo(currentDependency);    // If the component is in update list, then return the one with new tag, otherwise, return itself

        // If the tag of this component isn't same with that of this component in updated list
        if (currentDependency.getTag() != c.getTag())   // So if their tags are different, it means this dependency changes
        {
            if (component.isPackage(repoEnv))    // Component is package
            {
                if (component.hasSourceCode(packagesWithoutSourceCode))    // Component is package, and has source code
                {
                    ComponentSrcDir componentSrcDir(TMP_COMPONENT_DIR, component.getName(), component.getTag());
                    componentSrcDir.init();

                    if (!componentSrcDir.canMakePkg())    // Component is package, has source code, but can't make package
                    {
                        throw MyError(-1, "This package has source code, but can't make package", __LINE__, __FUNCTION__);
                    }
                }
                else    // Component is package, but doesn't have source code
                {
                    ComponentPkgDir componentPkgDir(TMP_COMPONENT_DIR, component.getName(), component.getTag());
                    componentPkgDir.init();
                }
            }
            else    // Component is source
            {
                ComponentSrcDir componentSrcDir(TMP_COMPONENT_DIR, component.getName(), component.getTag());
                componentSrcDir.init();
            }

            RepoManifest repoManifest(TMP_COMPONENT_DIR + "/" + component.getName());
            repoManifest.updateDependency(currentDependency.getName(), currentDependency.getTag(), c.getTag());

            component.setCommitMessageOfDependency(dependencies[i], c);  // Write commit message of dependency

            component.setUpdated(true);
        }
    }

    if (component.needToBeUpdated()) {

        if (component.getName() != dependencyPyramid[0][0].getName())
            component.updateTag();  // Just update tag in object, not create a real tag in component dir

        if (component.isPackageWithoutSourceCode(repoEnv, packagesWithoutSourceCode))   // For package that we don't its source code
        {
            RepoManifest repoManifest(TMP_COMPONENT_DIR + "/" + component.getName());
            repoManifest.updateVersionTo(component.getTag());

            // TODO: update the changelog
        }
        else
        {
            // If the manifest is changed because its dependency changes, but not specified by user.
            // We don't know to which branch to commit, so we need user to input the branch here.
            // Check if component know which branch to commit
            if (component.getBranchToCommit().isEmpty())  // This component doesn't know which branch to commit
            {
                // TODO: don't use a public variable componentNeedsBranch

                componentNeedsBranch = component.getName();
                emit requestBranchDialog();
                complete.lock();
                waitCondition.wait(&complete);
                complete.unlock();

                component.setBranchToCommit(branchInputInDialog);
            }

            component.commitChangeOfManifest();

            if (component.getName() != dependencyPyramid[0][0].getName()) {     // For now, we don't create a new tag for the root component, as such Esmeralda
                component.creatNewTag();    // Creat a new tag for repo accroding the member tag
            }
        }
    }

    return component;
}

Component Dot::componentSpecifiedTo(Component component)
{
    for (int i = 0; i < componentsToUpdate.size(); i++)
    {
        if (component.getName() == componentsToUpdate[i].getName()) {   // If this component is in the update list
            QString newTag = componentsToUpdate[i].getTag();
            QString branchToCommit = componentsToUpdate[i].getBranchToCommit();

            qDebug() << component.getName() << " is specified to " << newTag << " in update list";

            component.setTag(newTag);   // Change its tag, so the caller will get a component with both new tag and dependencies. component is a local variable, so it's safe to change its tag here.
            component.setBranchToCommit(branchToCommit);    // We also need to know which branch we should commit to
            break;
        }
    }

    return component;
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

