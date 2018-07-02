#include "gitexecutor.h"
#include <QDebug>
#include <QDir>

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

    QDir d(dir);
    if (!d.exists()) {
        qDebug() << dir << "doesn't exist";
        return -1;
    }
    setWorkingDirectory(dir);

    ret = clone(repo);

    return ret;
}

int GitExecutor::checkout(QString ref)
{
    int ret = 0;

    cmd = QString("git checkout %1").arg(ref);
    qDebug() << "Running " << cmd;

    ret = executeCmd();
    if (ret < 0) {
        qDebug() << "Failed to " << cmd;
    }

    return ret;
}

int GitExecutor::checkoutInDir(QString ref, QString dir)
{
    int ret = 0;

    QDir d(dir);
    if (!d.exists()) {
        qDebug() << dir << "doesn't exist";
        return -1;
    }

    setWorkingDirectory(dir);

    ret = checkout(ref);

    return ret;
}

void GitExecutor::setCmd(QString cmd)
{
    cmd = "git " + cmd;     // not used for now
}
