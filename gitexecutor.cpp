#include "gitexecutor.h"
#include <QDebug>

GitExecutor::GitExecutor()
{
    gitRepoAddress = defaultGitRepoAddress;
}

GitExecutor::GitExecutor(QString gitRepoAddress)
{
    this->gitRepoAddress = gitRepoAddress;
}

GitExecutor::~GitExecutor()
{

}

int GitExecutor::clone(QString repo)
{
    int ret = 0;

    cmd = QString("git clone %1%2").arg(gitRepoAddress).arg(repo);
    qDebug() << "Running " << cmd;

    ret = executeCmd();

    return ret;
}

int GitExecutor::cloneInDir(QString repo, QString dir)
{
    int ret = 0;

    setWorkingDirectory(dir);

    ret = clone(repo);

    return ret;
}

void GitExecutor::setCmd(QString cmd)
{
    cmd = "git " + cmd;     // not used for now
}
