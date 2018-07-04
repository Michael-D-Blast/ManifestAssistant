#include "gitexecutor.h"
#include <QDebug>
#include <QDir>
#include "myerror.h"

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

QStringList GitExecutor::getBranches()
{
    QStringList branches;

    // Run git branch -a to get all the branches
    cmd = QString("git branch -a");

    try {
        branches = executeCmdAndReturnOutput();
    }
    catch (MyError e) {
        throw;
    }

    QString b;
    for (int i = 0; i < branches.size(); i++) {
        b = branches.at(i);
        if (b.contains("remote") && !b.contains("->")) {
            branches[i] = b.section('/', 2, 2);
        }
    }

    return branches;
}

QStringList GitExecutor::getBranchesInDir(QString dir)
{
    QStringList branches;

    QDir d(dir);
    if (!d.exists()) {
        qDebug() << dir << "doesn't exist";
        throw MyError(-1, dir + "doesn't exist", __LINE__, __FUNCTION__);
    }
    setWorkingDirectory(dir);

    try {
        branches = getBranches();
    }
    catch (MyError e) {
        throw;
    }

    return branches;
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