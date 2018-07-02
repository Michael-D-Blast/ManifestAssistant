#include "component.h"
#include <QDebug>
#include "dot.h"
#include <QProcess>
#include "cmdexecutor.h"
#include "gitexecutor.h"
#include <QDir>

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

void Component::setName(QString nameToBe)
{
    name = nameToBe;
}

void Component::setTag(QString tagToBe)
{
    tag = tagToBe;
}

QString Component::getName() const
{
    return name;
}

QString Component::getTag() const
{
    return tag;
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
    qDebug() << "Dummy: updating " << newDependency.getName() << " to " << newDependency.getTag() << " in the manifest of " << name;

    QString fileName = TMP_COMPONENT_DIR + "/" + name + "/" + "repo-manifest";
    QFile file(fileName);

    if (!file.exists()) {
        qDebug() << fileName << " doesn't exist";
        GitExecutor gitExecutor;
        gitExecutor.cloneInDir(name, TMP_COMPONENT_DIR);
        gitExecutor.checkoutInDir(tag, TMP_COMPONENT_DIR + "/" + name);
    }

    // TODO: Assmeble a class to do this
    CmdExecutor cmdExecutor;
    QString cmd = QString("sed -i {/%1/s/%2/%3/} %4").arg(newDependency.getName()).arg(oldDependency.getTag()).arg(newDependency.getTag()).arg(fileName);
    qDebug() << "Running " << cmd;
    cmdExecutor.setCmd(cmd);
    cmdExecutor.executeCmdInDir(TMP_COMPONENT_DIR + "/" + name);

    return 0;
}

// It's not allowed to use the tag in this method;
int Component::updateBuildInManifest()
{
    qDebug() << "Dummy: updating the tag of " << name;

    return 0;
}

// It's not allowed to use the tag in this method;
int Component::commitChangeOfManifest()
{
    qDebug() << "Dummy: commit change of manifest";
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

