#include "manifesteditor.h"
#include <QFile>
#include <myerror.h>

ManifestEditor::ManifestEditor()
{

}

ManifestEditor::ManifestEditor(QString workdir, QString component) :
    FileEditor(workdir + "/" + component + "/repo-manifest")
{

}

QString ManifestEditor::getBuildInManifest() const
{
    QString build = "";

    try {
        build = getValueOfKey("BUILD", '=');
    }
    catch (MyError e) {
        throw;
    }

    return build;
}

int ManifestEditor::updateDependencyTag(const QString &oldTag, const QString &newTag, const QString &dependency)
{
    return (updateValueInLinesContainingKeyword(oldTag, newTag, dependency));
}

int ManifestEditor::updateBuild()
{

}
