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
        if (exists())
        {
            git.fetch(getWorkspacePath() + "/" + getName());
        }
        else
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

    // TODO: this command takes too long, display the process to a status window

    CmdExecutor cmd(QString("jbuild -c -T %1 %2").arg(getName()).arg(getTag()));
    try {
        cmd.execute(TMP_COMPONENT_DIR);
    } catch (MyError e) {
        e.displayError();
        throw;
    }
}
