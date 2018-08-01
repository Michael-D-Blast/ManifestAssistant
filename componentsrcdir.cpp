#include "componentsrcdir.h"
#include "gitexecutor.h"
#include "myerror.h"
#include "cmdexecutor.h"
#include <QDebug>

ComponentSrcDir::ComponentSrcDir(QString workspacePath, QString componentName, QString componentTag) :
        ComponentDir(workspacePath, componentName, componentTag)
{

}

bool ComponentSrcDir::canMakePkg() const
{
    return exists("Makefile.mk");
}

void ComponentSrcDir::init()
{
    GitExecutor git;

    try
    {
        if (!exists())
        {
            git.clone(getName(), getWorkspacePath());
        }

        git.checkout(getTag(), getWorkspacePath() + "/" + getName());
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}

void ComponentSrcDir::makePackage()
{
    qDebug() << "Making package " << getName() << " (has source code)";

    CmdExecutor cmd(QString("jbuild -c -T %1 %2").arg(getName()).arg(getTag()));
    try {
        cmd.execute(TMP_COMPONENT_DIR);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}
