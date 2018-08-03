#include "repoexecutor.h"
#include "cmdexecutor.h"
#include "myerror.h"
#include <QDebug>

RepoExecutor::RepoExecutor()
{

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
