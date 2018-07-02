#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H

#include <QProcess>

class CmdExecutor : public QProcess
{
public:
    CmdExecutor();
    CmdExecutor(QString command);
    ~CmdExecutor();

    // Set Methods
    void setCmd(QString cmd);
    int executeCmd();
    int executeCmdInDir(QString dir);

private:
    QString cmd;
};

#endif // CMDEXECUTOR_H
