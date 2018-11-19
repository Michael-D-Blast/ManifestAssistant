#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <QString>

class FileEditor
{
public:
    FileEditor();
    FileEditor(QString file);
    ~FileEditor();

    // Get methods
    // If the file has a line like <KEY> = <VALUE>, then return <VALUE> of the first <KEY>. Ignores the space.
    QString getValueOfKey(QString key, QChar sep) const;

    // Set methods
    int updateValueInLinesContainingKeyword(const QString &oldValue, const QString &newValue, const QString &keyword);

private:
    QString file;
};

#endif // FILEEDITOR_H
