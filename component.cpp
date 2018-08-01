#include "component.h"
#include <QDebug>
#include "dot.h"
#include <QProcess>
#include "cmdexecutor.h"
#include "gitexecutor.h"
#include <QDir>
#include "manifesteditor.h"
#include "branchdialog.h"
#include "myerror.h"

//#define DO_DUMMY_PROCESS

Component::Component()
{
    needUpdated = false;
}

Component::Component(QString nameOfComponent)
{
    name = nameOfComponent;
    needUpdated = false;
}

Component::Component(QString nameOfComponent, QString tagOfComponent)
{
    name = nameOfComponent;
    tag = tagOfComponent;
    needUpdated = false;
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

void Component::setUpdated(bool yn)
{
    needUpdated = yn;
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

bool Component::needToBeUpdated() const
{
    return needUpdated;
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
    try {
        gitExecutor.clone(name, TMP_COMPONENT_DIR);
        gitExecutor.checkout(tag, TMP_COMPONENT_DIR + "/" + name);
    } catch (MyError e) {
        e.displayError();
        return -1;
    }

    return ret;
}

void Component::appendDependency(ComponentsList dependencies)
{
    this->dependencies << dependencies;
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
        gitExecutor.clone(name, TMP_COMPONENT_DIR);
        gitExecutor.checkout(tag, TMP_COMPONENT_DIR + "/" + name);
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

int Component::updateBuildInManifest()
{
    QString newBuild = tag.section('.', 3, 3);

    qDebug() << "New Build is " << newBuild;
    // Update BUILD in current manifest
    ManifestEditor manifest(TMP_COMPONENT_DIR, name);

    return (manifest.updateBuild(newBuild));
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
    try {
        gitExecutor.checkout(branchToCommit, TMP_COMPONENT_DIR + "/" + name);
    } catch (MyError e) {
        e.displayError();
        return -1;
    }

    QString commitMessageFile = TMP_COMPONENT_DIR + "/commit_messages/" + name;
    QFile f(commitMessageFile);
    if (!f.exists()) {
        qDebug() << commitMessageFile << " doesn't exist!";
        return -1;
    }

    // Git commit
    try {
        gitExecutor.commit("repo-manifest", commitMessageFile , TMP_COMPONENT_DIR + "/" + name);
    } catch (MyError e) {
        e.displayError();
        return -1;
    }

    return 0;
}

int Component::creatNewTag()
{
#ifdef DO_DUMMY_PROCESS
    qDebug() << "Dummy: creat a new tag for " << name;

    return 0;
#endif
    int ret = 0;
    GitExecutor gitExecutor;

    // Git tag
    try {
        gitExecutor.tag(tag, TMP_COMPONENT_DIR + "/" + name);
    } catch (MyError e) {
        e.displayError();
        return -1;
    }

    return ret;
}

void Component::updateTag(void)
{
    QString newTag = Dot::updateTag(tag);
    tag = newTag;
}

int Component::generateCommitMessageFileBetweenTags(QString oldTag, QString newTag)
{
    QString log;

    QFile file(TMP_COMPONENT_DIR + "/commit_messages/" + name);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Can't open " << TMP_COMPONENT_DIR << "/commit_messages/" << name;
        return -1;
    }

    // Run git log to get commit log
    GitExecutor git;
    log = git.getLog(oldTag, newTag, TMP_COMPONENT_DIR + "/" + name);

    qDebug() << "Log got is " << log;

    QTextStream in(&file);
    in << log << endl;

    file.close();
}

// TODO: Maybe we don't need two arguments

int Component::setCommitMessageOfDependency(Component oldDependency, Component newDependency)
{
    QString componentFileName = TMP_COMPONENT_DIR + "/commit_messages/" + name;
    QFile componentFile(componentFileName);

    QString dependencyFileName = TMP_COMPONENT_DIR + "/commit_messages/" + oldDependency.getName();
    QFile dependencyFile(dependencyFileName);

    // TODO: Check if we can change the mode to WriteOnly

    if (!componentFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open " << componentFileName;

        return -1;
    }

    if (!dependencyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open " << dependencyFileName;

        componentFile.close();
        return -1;
    }

    QTextStream c(&componentFile);
    QTextStream d(&dependencyFile);
    d.seek(0);

    QString bump = "Bump " + oldDependency.getName() + " from " + oldDependency.getTag() + " to " + newDependency.getTag();
    c << bump << endl;
    c << "Changes in " << oldDependency.getName() << " : " << endl;

    while(!d.atEnd()) {
        QString line;
        line = d.readLine();
        line = "    " + line;
        qDebug() << "Read " << line << " from " << dependencyFileName;
        c << line << endl;
    }

    componentFile.close();
    dependencyFile.close();

    return 0;
}

void Component::displayDependencies() const
{
    for (int i = 0; i < dependencies.size(); i++)
    {
        qDebug() << "\t" << dependencies.at(i).getName() << "@" << dependencies.at(i).getTag();
    }
}

