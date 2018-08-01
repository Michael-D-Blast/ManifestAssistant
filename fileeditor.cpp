#include "fileeditor.h"
#include "cmdexecutor.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include "myerror.h"

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

QString FileEditor::getValueOfKey(QString key, QChar sep) const
{
    QString value = "";

    QFile f(file);
    if(!f.open(QIODevice::ReadOnly)) {
        throw MyError(-1, "Failed to open " + file, __LINE__, __FUNCTION__);
    }

    QTextStream s(&f);
    QString line;
    while(!s.atEnd()) {
        line = s.readLine();
        if (line.contains(key)) {

            qDebug() << "Find " << line;

            line.remove(" ");
            value = line.section(sep, 1, 1);
            break;
        }
    }

    f.close();

    return value;   // If key isn't found, return an empty string
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

    CmdExecutor c(cmd);
    try {
        c.execute();
    } catch (MyError e) {
        e.displayError();
        return -1;
    }

    return 0;
}
