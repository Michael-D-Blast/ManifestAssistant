#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <QString>

class FileEditor
{
public:
    FileEditor();
    FileEditor(QString file);
    ~FileEditor();

    // Set methods
    int updateValueInLinesContainingKeyword(const QString &oldValue, const QString &newValue, const QString &keyword);

private:
    QString file;
};

#endif // FILEEDITOR_H
