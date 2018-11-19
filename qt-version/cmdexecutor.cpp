#include "cmdexecutor.h"
#include "QDebug"
#include <QByteArray>
#include "myerror.h"
#include <QDir>


CmdExecutor::CmdExecutor(QString command)
{
    this->cmd = command;
}

QStringList CmdExecutor::execute(QString dir, unsigned int timeoutInMs)
{
    QStringList output;

    if (!dir.isEmpty())
    {
        QDir d(dir);
        if (!d.exists())
        {
            throw MyError(-1, dir + "doesn't exist", __LINE__, __FUNCTION__);
        }

        setWorkingDirectory(dir);
    }

    start(cmd);

    if (!waitForStarted()) {
        throw MyError(-1, "Failed to start " + cmd, __LINE__, __FUNCTION__);
    }

    if (!waitForFinished(timeoutInMs)) {
        throw MyError(-2, "Failed to finish " + cmd, __LINE__, __FUNCTION__);
    }

    int ret;
    if ((ret = exitCode()) != 0) {
        qDebug() << "Standard error: " << QString(readAllStandardError());
        throw MyError(-3, "Failed to run " + cmd, __LINE__, __FUNCTION__);
    }

    output = QString(readAllStandardOutput()).split("\n");

    return output;
}
