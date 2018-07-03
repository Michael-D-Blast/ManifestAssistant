#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H

#include <QProcess>
#include <QStringList>

class CmdExecutor : public QProcess
{
public:
    CmdExecutor();
    CmdExecutor(QString command);
    ~CmdExecutor();

    // Set Methods
    virtual void setCmd(QString cmd);
    int executeCmd();
    int executeCmdInDir(QString dir);

    QStringList executeCmdAndReturnOutput();

protected:
    QString cmd;

private:
};

#endif // CMDEXECUTOR_H
