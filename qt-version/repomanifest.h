#ifndef REPOMANIFEST_H
#define REPOMANIFEST_H

#include <QFile>
#include "fileeditor.h"

class RepoManifest : public QFile, public FileEditor
{
public:
    explicit RepoManifest(QString fileDir);

    void updateDependency(QString dependencyName, QString oldTag, QString newTag);
    void updateDependency(QString dependencyName, QString newTag);

    // TODO: this is only available for repo-manifest in pkg. Create two child classes, and change it to virtual

    void updateVersionTo(QString newVersion);
};

#endif // REPOMANIFEST_H
