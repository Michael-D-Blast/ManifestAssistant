#include "component.h"
#include <QDebug>
#include "dot.h"
#include <QProcess>
#include "cmdexecutor.h"
#include "gitexecutor.h"

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

    qDebug() << "Dummy: checkout " << name << " to " << tag;

    // TODO: Check if the component's working directory is clean.
    // If it is, we do the checkout in current directory, otherwise, we do the checkout in a temp directory.
    // Now, we only do it in a temp directory.
    GitExecutor gitExecutor;
    ret = gitExecutor.cloneInDir(name, TMP_COMPONENT_DIR);
    if (ret < 0) {
        qDebug() << "Failed to git clone " << name;
    }

    return ret;
}

void Component::appendDependency(Component dependentComponent)
{
    dependencies << dependentComponent;
}

int Component::updateDependencyInManifest(Component dependency)
{
    qDebug() << "Dummy: updating " << dependency.getName() << " to " << dependency.getTag() << " in the manifest of " << name;

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

