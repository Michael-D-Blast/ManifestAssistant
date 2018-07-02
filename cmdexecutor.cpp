#include "cmdexecutor.h"
#include "QDebug"

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

    if (!waitForFinished()) {
        qDebug() << "Failed to finish cmd";
        return -2;
    }

    return 0;
}

int CmdExecutor::executeCmdInDir(QString dir)
{
    int ret;

    setWorkingDirectory(dir);

    ret = executeCmd();

    return ret;
}
