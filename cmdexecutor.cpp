#include "cmdexecutor.h"
#include "QDebug"
#include <QByteArray>
#include "myerror.h"

CmdExecutor::CmdExecutor()
{

}

CmdExecutor::CmdExecutor(QString command)
{
    this->cmd = command;
}

CmdExecutor::~CmdExecutor()
{

}

void CmdExecutor::setCmd(QString cmd)
{
    this->cmd = cmd;
}

int CmdExecutor::executeCmd()
{
    start(cmd);

    if (!waitForStarted()) {
        qDebug() << "Failed to start cmd";
        return -1;
    }

	// TODO: Don't use magic number
    if (!waitForFinished(10 * 60 * 1000)) {
        qDebug() << "Failed to finish cmd";
        return -2;
    }

    int ret = exitCode();
    if (ret != 0) {
        qDebug() << "Standard error: " << QString(readAllStandardError());
    }

    qDebug() << "Standard output: " << QString(readAllStandardOutput());

    return ret;
}

int CmdExecutor::executeCmdInDir(QString dir)
{
    int ret;

    setWorkingDirectory(dir);

    ret = executeCmd();

    return ret;
}

QStringList CmdExecutor::executeCmdAndReturnOutput()
{
    QStringList output;

    start(cmd);

    if (!waitForStarted()) {
        qDebug() << "Failed to start cmd";
        throw MyError(-1, "Failed to start " + cmd, __LINE__, __FUNCTION__);
    }

    // TODO: Don't use magic number
    if (!waitForFinished(10 * 60 * 1000)) {
        qDebug() << "Failed to finish cmd";
        throw MyError(-2, "Failed to finish " + cmd, __LINE__, __FUNCTION__);
    }

    int ret = exitCode();
    if (ret != 0) {
        qDebug() << "Standard error: " << QString(readAllStandardError());
        throw MyError(-3, "Failed to run " + cmd, __LINE__, __FUNCTION__);
    }

    output = QString(readAllStandardOutput()).split("\n");

    return output;
}
