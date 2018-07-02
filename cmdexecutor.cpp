#include "cmdexecutor.h"
#include "QDebug"
#include <QByteArray>

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

    QByteArray output, error;
    output = readAllStandardOutput();
    error = readAllStandardError();
    qDebug() << "output: " << QString(output);
    qDebug() << "error: " << QString(error);

    return 0;
}

int CmdExecutor::executeCmdInDir(QString dir)
{
    int ret;

    setWorkingDirectory(dir);

    ret = executeCmd();

    return ret;
}
