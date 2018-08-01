#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QList>
#include <QStringList>
#include "repoenv.h"


static const QString TMP_COMPONENT_DIR = "/tmp/components";

typedef QList<class Component> ComponentsList;
typedef QList<ComponentsList> ComponentsMesh;

class Component
{
public:
    Component();
    Component(QString nameOfComponent);
    Component(QString nameOfComponent, QString tagOfComponent);
    ~Component();

    // Get Methods
    QString getName() const;
    QString getTag() const;
    QString getBranchToCommit() const;
    bool needToBeUpdated() const;
    ComponentsList getDependencies();
    virtual bool is(Component anotherComponent);
    void displayDependencies() const;
    bool isPackage(const RepoEnv *repoEnv) const;
    bool hasSourceCode(const QStringList &packagesWithoutSourceCode) const;
    bool isPackageWithoutSourceCode(const RepoEnv *repoEnv, const QStringList &packagesWithoutSourceCode) const;

    // Set Methods
    void setName(QString name);
    void setTag(QString tag);
    void setBranches(QStringList branches);
    void setBranchToCommit(QString branchToCommit);
    void setUpdated(bool yn);
    int checkoutToTag();
    void appendDependency(ComponentsList dependencies);
    int updateBuildInManifest();    // Update BUILD in manifest
    int commitChangeOfManifest();
    int creatNewTag();
    void updateTag(void);
    int generateCommitMessageFileBetweenTags(QString oldTag, QString newTag);    // For components whose souce code has been changed
    int setCommitMessageOfDependency(Component oldDependency, Component newDependency);

private:
    QString name;
    QString tag;
    QStringList branches;   // All branches this component has, obsolete
    QString branchToCommit; // The branch we want to commit
    ComponentsList dependencies;   // Just record the component's name, because we don't care the tag in dependencies
    bool needUpdated;    // If it true, the component should be pushed to remote. Default value is false
};


#endif // COMPONENT_H
