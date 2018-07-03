#include "manifesteditor.h"

ManifestEditor::ManifestEditor()
{

}

ManifestEditor::ManifestEditor(QString workdir, QString component) :
    FileEditor(workdir + "/" + component + "/repo-manifest")
{

}

int ManifestEditor::updateDependencyTag(const QString &oldTag, const QString &newTag, const QString &dependency)
{
    return (updateValueInLinesContainingKeyword(oldTag, newTag, dependency));
}

int ManifestEditor::updateBuild()
{

}
