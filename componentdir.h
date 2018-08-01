#ifndef COMPONENTDIR_H
#define COMPONENTDIR_H

#include <QDir>
#include "component.h"
#include "workspace.h"
#include "repomanifest.h"

// TODO: Refactor component, we just contain the basic name and tag

class ComponentDir : public Component, public QDir
{
public:
    explicit ComponentDir(QString workspacePath, QString componentName, QString componentTag);
    ~ComponentDir();

    QString getWorkspacePath() const;

    virtual void init();
    virtual void makePackage();

private:
    Workspace *workspace;
    RepoManifest *repoManifest;
};

#endif // COMPONENTDIR_H
