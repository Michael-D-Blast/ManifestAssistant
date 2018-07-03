#include "fileeditor.h"
#include "cmdexecutor.h"
#include <QDebug>
#include <QFileInfo>

FileEditor::FileEditor()
{

}

FileEditor::FileEditor(QString file)
{
    this->file = file;
}

FileEditor::~FileEditor()
{

}

int FileEditor::updateValueInLinesContainingKeyword(const QString &oldValue, const QString &newValue, const QString &keyword)
{
    QFileInfo f(file);

    if (!f.exists()) {
        qDebug() << file << "doesn't exist!";
        return -1;
    }

    QString cmd = QString("sed -i {/%1/s/%2/%3/} %4").arg(keyword).arg(oldValue).arg(newValue).arg(file);
    qDebug() << "Running " << cmd;

    CmdExecutor cmdExecutor;
    cmdExecutor.setCmd(cmd);

    return(cmdExecutor.executeCmd());
}
