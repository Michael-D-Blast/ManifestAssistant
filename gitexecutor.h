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

    int clone(QString repo);
    int cloneInDir(QString repo, QString dir);

protected:
    void setCmd(QString cmd);

private:
    QString gitRepoAddress;
};

#endif // GITEXECUTOR_H
