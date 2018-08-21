#include "componentpkgdir.h"
#include "cmdexecutor.h"
#include <stdlib.h>
#include "myerror.h"
#include <QDebug>

ComponentPkgDir::ComponentPkgDir(QString workspacePath, QString componentName, QString componentTag) :
    ComponentDir(workspacePath, componentName, componentTag)
{

}

void ComponentPkgDir::init()
{
    try
    {
        // There is normal case that the dir has already existed,
        // which is this is component is already specified by user,
        // so it has already been downloaded as we need to get the commit message
        // from the source code.
        // This is found in issue #1
        // The current solution is removing the previous dir which contains the source code
        if (exists()) {
            qDebug() << "NOTICE:" + getName() + " has already existed, removing it!";
            qDebug() << "(The possible reason is this component is a package, "
                        "it has been specified by user and its dependency needs to update)";

            if (!removeRecursively())
                throw MyError(-1, "Failed to remove " + getName() + " in " + getWorkspacePath(), __LINE__, __FUNCTION__);
        }

        QString home(getenv("HOME"));
        CmdExecutor cmd(QString("tar -xzf %1/.repo/cache/%2-%3.rpk -C %4").
                        arg(home).arg(getName()).arg(getTag()).arg(getWorkspacePath()));
        cmd.execute();

    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}

void ComponentPkgDir::makePackage()
{
    qDebug() << "Making package " << getName() << " (no source code)";

    CmdExecutor tar(QString("tar -czf %1-%2.rpk %3").arg(getName()).arg(getTag()).arg(getName()));
    CmdExecutor curl(QString("curl -T %1-%2.rpk ftp://abbgit:abb@ftp.abbszswrd.com").arg(getName()).arg(getTag()));

    try
    {
        tar.execute(getWorkspacePath());
        curl.execute(getWorkspacePath());
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}
