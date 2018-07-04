#include "component.h"
#include <QDebug>
#include "dot.h"
#include <QProcess>
#include "cmdexecutor.h"
#include "gitexecutor.h"
#include <QDir>
#include "manifesteditor.h"
#include "branchdialog.h"

//#define DO_DUMMY_PROCESS

Component::Component()
{

}

Component::Component(QString nameOfComponent)
{
    name = nameOfComponent;

}

Component::Component(QString nameOfComponent, QString tagOfComponent)
{
    name = nameOfComponent;
    tag = tagOfComponent;

}

Component::~Component()
{

}

void Component::setName(QString name)
{
    this->name = name;
}

void Component::setTag(QString tag)
{
    this->tag = tag;
}

void Component::setBranches(QStringList branches)
{
    this->branches = branches;
}

void Component::setBranchToCommit(QString branchToCommit)
{
    this->branchToCommit = branchToCommit;
}

QString Component::getName() const
{
    return name;
}

QString Component::getTag() const
{
    return tag;
}

QString Component::getBranchToCommit() const
{
    return branchToCommit;
}

ComponentsList Component::getDependencies()
{
    return dependencies;
}

bool Component::is(Component anotherComponent)
{
    return(name == anotherComponent.getName());
}

int Component::checkoutToTag()
{
#ifdef DO_DUMMY_PROCESS
    qDebug() << "Checkouting " << name << " to " << tag;

    return 0;
#endif
    int ret = 0;

    qDebug() << "Checkouting " << name << " to " << tag;

    // TODO: Check if the component's working directory is clean.
    // If it is, we do the checkout in current directory, otherwise, we do the checkout in a temp directory.
    // Now, we only do it in a temp directory.

    // If the component repo has already existed in tmp dir, remove it first
    // TODO: Assemble a new dir class to check if a dir exist and remove it

    QDir dir(TMP_COMPONENT_DIR + "/" + name);
    dir.removeRecursively();

    GitExecutor gitExecutor;
    ret = gitExecutor.cloneInDir(name, TMP_COMPONENT_DIR);
    if (ret < 0) {
        qDebug() << "Failed to git clone " << name;

        return -1;
    }

    ret = gitExecutor.checkoutInDir(tag, TMP_COMPONENT_DIR + "/" + name);
    if (ret < 0) {
        qDebug() << "Failed to git checkout " << tag;

        return -2;
    }

    return ret;
}

void Component::appendDependency(Component dependentComponent)
{
    dependencies << dependentComponent;
}

int Component::updateDependencyInManifest(Component oldDependency, Component newDependency)
{
#ifdef DO_DUMMY_PROCESS
    qDebug() << "In manifest of " << name << ", update " << oldDependency.getName() << " from " << oldDependency.getTag() << " to " << newDependency.getTag();

    return 0;
#endif
    QString dependency = oldDependency.getName();
    QString oldTag = oldDependency.getTag();
    QString newTag = newDependency.getTag();

    QString fileName = TMP_COMPONENT_DIR + "/" + name + "/" + "repo-manifest";
    QFile file(fileName);

    if (!file.exists()) {
        qDebug() << fileName << " doesn't exist";
        GitExecutor gitExecutor;
        gitExecutor.cloneInDir(name, TMP_COMPONENT_DIR);
        gitExecutor.checkoutInDir(tag, TMP_COMPONENT_DIR + "/" + name);
    }

    // Update the dependency's tag in repo-manifest
    ManifestEditor manifest(TMP_COMPONENT_DIR, name);
    int ret;

    if ((ret = manifest.updateDependencyTag(oldTag, newTag, dependency)) < 0) {
        qDebug() << "Failed to update the tag of " << dependency << " from " << oldTag << " to" << newTag << " in the manifest of " << name;
        return -1;
    }

    return 0;
}

QString Component::updateBuildInManifest()
{
    // I thought the tag is VERSION.BUILD, but in fact, BUILD is always 0. So we don't update it in manifest.
#if 0
    QString newBuild = "";
    // Update BUILD in current manifest

    // Check if the tag hold by this object is same with the one in repo-manifest
    ManifestEditor manifest(TMP_COMPONENT_DIR, name);
    QString currentBuild = manifest.getBuildInManifest();
    QString tagInManifest = manifest.getVersionInManifest() + "." + currentBuild;

    if (tag != tagInManifest && tag != "master") {  // If VERSION.BUILD is different from the tag we hold and we aren't in tag
        qDebug() << "Current tag is " << tag << ", but tag in manifest is " << tagInManifest;
    }

    qDebug() << "Current BUILD is " << currentBuild;

    return newBuild;
#endif
}

int Component::commitChangeOfManifest()
{
#ifdef DO_DUMMY_PROCESS
    qDebug() << "Dummy: commit change of manifest";

    return 0;
#endif
    // Git checkout to the specified branch
    int ret;
    GitExecutor gitExecutor;
    ret = gitExecutor.checkoutInDir(branchToCommit, TMP_COMPONENT_DIR + "/" + name);
    if (ret != 0) {
        qDebug() << "Failed to checkout to branch " << branchToCommit;
        return ret;
    }

    // Git commit
    ret = gitExecutor.commitInDir("Test commit", TMP_COMPONENT_DIR + "/" + name);
    if (ret != 0) {
        qDebug() << "Failed to commit to branch " << branchToCommit;
        return ret;
    }

    return 0;
}

// It's not allowed to use the tag in this method;
int Component::creatNewTag()
{
    qDebug() << "Dummy: creat a new tag for " << name;
}

void Component::updateTag(void)
{
    QString newTag = Dot::updateTag(tag);
    tag = newTag;
}

void Component::displayDependencies() const
{
    for (int i = 0; i < dependencies.size(); i++)
    {
        qDebug() << "\t" << dependencies.at(i).getName() << "@" << dependencies.at(i).getTag();
    }
}

