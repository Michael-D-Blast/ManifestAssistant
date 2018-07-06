#ifndef GITEXECUTOR_H
#define GITEXECUTOR_H

#include "cmdexecutor.h"

static const QString defaultGitRepoAddress = "git@git.abbszswrd.com:TerraZ63_ARM/";

class GitExecutor : public CmdExecutor
{
public:
    GitExecutor();
    GitExecutor(QString gitRepoAddress);
    ~GitExecutor();

    // Get Methods
    QStringList getBranches();
    QStringList getBranchesInDir(QString dir);
    QString getCurrentBranch();
    QString getCurrentBranchInDir(QString dir);

    // Set Methods
    int clone(QString repo);
    int cloneInDir(QString repo, QString dir);
    int checkout(QString ref);
    int checkoutInDir(QString ref, QString dir);
    int commit(QString file, QString commitMessageFile);    // Not only commit, but also git add the file first
    int commitInDir(QString file, QString commitMessageFile, QString dir);
    int push(QString branch);    // Assume the name of local bransh is same with remote branch
    int pushInDir(QString branch, QString dir);
    int tag(QString newTag);
    int tagInDir(QString newTag, QString dir);
    QString getLog(QString oldTag, QString newTag);
    QString getLogInDir(QString oldTag, QString newTag, QString dir);

protected:
    void setCmd(QString cmd);

private:
    QString gitRepoAddress;
};

#endif // GITEXECUTOR_H
