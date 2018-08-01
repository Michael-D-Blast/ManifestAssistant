#ifndef MANIFESTEDITOR_H
#define MANIFESTEDITOR_H

#include "fileeditor.h"

// TODO: Remove this class, use RepoManifest

class ManifestEditor : public FileEditor
{
public:
    ManifestEditor();
    ManifestEditor(QString workdir, QString component);

    // Get Methods
    // Return the BUILD in repo-manifest
    QString getBuildInManifest() const;
    QString getVersionInManifest() const;

    // Set Methods
    int updateDependencyTag(const QString &oldTag, const QString &newTag, const QString &dependency);
    int updateBuild(const QString newBuild);

private:

};

#endif // MANIFESTEDITOR_H
