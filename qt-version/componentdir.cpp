#include "componentdir.h"

ComponentDir::ComponentDir(QString workspacePath, QString componentName, QString componentTag) :
    Component(componentName, componentTag),
    QDir(workspacePath + "/" + componentName)
{
    workspace = new Workspace(workspacePath);
    repoManifest = new RepoManifest(workspacePath + "/" + componentName);
}

ComponentDir::~ComponentDir()
{
    delete workspace;
    delete repoManifest;
}

QString ComponentDir::getWorkspacePath() const
{
    return workspace->absolutePath();
}

void ComponentDir::init()
{

}

void ComponentDir::makePackage()
{

}

