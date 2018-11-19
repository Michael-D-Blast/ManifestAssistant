#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H

#include <QProcess>
#include <QStringList>

const unsigned int CMD_TIMEOUT_IN_MS = 10 * 60 * 1000;

class CmdExecutor : public QProcess
{
public:
    explicit CmdExecutor(QString command);

    QStringList execute(QString dir = "", unsigned int timeoutInMs = CMD_TIMEOUT_IN_MS);

private:
    QString cmd;

};

#endif // CMDEXECUTOR_H
