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
        if (!exists())
        {
            QString home(getenv("HOME"));
            CmdExecutor cmd(QString("tar -xzf %1/.repo/cache/%2-%3.rpk -C %4").
                            arg(home).arg(getName()).arg(getTag()).arg(getWorkspacePath()));
            cmd.execute();
        }
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
