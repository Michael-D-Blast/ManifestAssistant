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
    int commit(QString commitMessage);
    int commitInDir(QString commitMessage, QString dir);

protected:
    void setCmd(QString cmd);

private:
    QString gitRepoAddress;
};

#endif // GITEXECUTOR_H
