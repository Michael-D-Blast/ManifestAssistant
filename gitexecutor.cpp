#include "gitexecutor.h"
#include <QDebug>
#include <QDir>
#include "myerror.h"
#include "cmdexecutor.h"

GitExecutor::GitExecutor(QString gitRepoAddress)
{
    this->gitRepoAddress = gitRepoAddress;
}

QStringList GitExecutor::getBranches(QString dir)
{
    QStringList branches;
    QStringList cmdOutput;

    // Run git branch -a to get all the branches
    CmdExecutor cmd("git branch -a");

    try {
        cmdOutput = cmd.execute(dir);
    }
    catch (MyError e) {
        e.displayError();
        throw;
    }

    QString line;
    for (int i = 0; i < cmdOutput.size(); i++) {
        line = cmdOutput.at(i);

        if (line.contains("remote") && !line.contains("->") && !line.contains("*")) {
            branches << line.section('/', 2, 2);
        }
    }

    return branches;
}

QString GitExecutor::getCurrentBranch(QString dir)
{
    QString currentBranch = "";
    QStringList output;

    CmdExecutor cmd("git branch");

    try {
        output = cmd.execute(dir);
    }
    catch (MyError e) {
        e.displayError();
        throw;
    }

    for (int i = 0; i < output.size(); i++) {
        if (output[i].contains("*")) {      // The one which contains * is current branch
            if (output[i].contains("detached")) {      // If it is in detached status, it's not in any branch, return empty
                return "";
            }
            currentBranch = output[i].section(' ', 1, 1);
        }
    }

    return currentBranch;
}

void GitExecutor::clone(QString repo, QString dir)
{
    CmdExecutor cmd("git clone " + gitRepoAddress + repo);

    qDebug() << "git clone " << repo << " ...";

    try {
        cmd.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

void GitExecutor::checkout(QString ref, QString dir)
{
    CmdExecutor cmd("git checkout " + ref);

    try {
        cmd.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

void GitExecutor::commit(QString file, QString commitMessageFile, QString dir)
{
    CmdExecutor cmdAdd("git add " + file);
    CmdExecutor cmdCommit("git commit -F " + commitMessageFile);

    try {
        cmdAdd.execute(dir);
        cmdCommit.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

void GitExecutor::push(QString branch, QString dir, QString remote)
{
    CmdExecutor cmd("git push " + remote + " " + branch + ":" + branch + " --tags");

    qDebug() << "git push " << branch << " ...";

    try {
        cmd.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

void GitExecutor::fetch(QString dir)
{
    CmdExecutor cmd("git fetch");

    qDebug() << "git fetch in " << dir << " ...";

    try {
        cmd.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

void GitExecutor::tag(QString newTag, QString dir)
{
    CmdExecutor cmd("git tag " + newTag);

    try {
        cmd.execute(dir);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}

QStringList GitExecutor::getLog(QString oldTag, QString newTag, QString dir)
{
    QStringList log;
    QStringList output;

    // TODO: record the whole log

    qDebug() << "git log --oneline --no-merges " << oldTag << ".." << newTag;

    CmdExecutor cmd("git log --oneline --no-merges " + oldTag + ".." + newTag);

    try
    {
        output = cmd.execute(dir);
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }

    for (int i = 0; i < output.size(); i++)
    {
        if (!output[i].isEmpty())
            log << output[i];
    }

    return log;
}
