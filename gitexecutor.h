#ifndef GITEXECUTOR_H
#define GITEXECUTOR_H

#include <QString>

static const QString DEFAULT_GIT_REPO_ADDRESS = "git@git.abbszswrd.com:TerraZ63_ARM/";

class GitExecutor
{
public:
    explicit GitExecutor(QString gitRepoAddress = DEFAULT_GIT_REPO_ADDRESS);

    // Get Methods
    QStringList getBranches(QString dir = "");
    QString getCurrentBranch(QString dir = "");

    // Set Methods
    void clone(QString repo, QString dir = "");
    void checkout(QString ref, QString dir = "");
    void commit(QString file, QString commitMessageFile, QString dir = "");    // Not only commit, but also git add the file first
    void push(QString branch, QString remote = "origin", QString dir = "");    // Assume the name of local bransh is same with remote branch
    void tag(QString newTag, QString dir = "");
    void fetch(QString dir = "");
    QString getLog(QString oldTag, QString newTag, QString dir = "");

private:
    QString gitRepoAddress;
};

#endif // GITEXECUTOR_H
