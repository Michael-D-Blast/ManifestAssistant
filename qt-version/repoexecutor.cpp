#include "repoexecutor.h"
#include "cmdexecutor.h"
#include "myerror.h"
#include <QDebug>

RepoExecutor::RepoExecutor()
{

}

// TODO: Add a arg to specify platform

void RepoExecutor::select(QString dir)
{
    CmdExecutor cmd("repo select");

    qDebug() << "repo select in " + dir + " ...";

    try
    {
        cmd.execute(dir);
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}

ComponentsList RepoExecutor::getList(QString dir)
{
    ComponentsList dependencies;
    QStringList output;
    CmdExecutor cmd("repo list");

    qDebug() << "repo list in " + dir + " ...";

    try
    {
        output = cmd.execute(dir);
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }

    // The first component is itself, so remote it.
    output.removeAt(0);

    QStringList::const_iterator i;
    for (i = output.constBegin(); i != output.constEnd(); ++i)
    {
        if (i->isEmpty())
            continue;

        Component c(i->section('/', 0, 0), i->section('/', 1, 1));
        dependencies << c;
    }

    return dependencies;
}
