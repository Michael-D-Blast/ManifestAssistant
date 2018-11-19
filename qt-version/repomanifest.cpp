#include "repomanifest.h"
#include "fileeditor.h"
#include "myerror.h"

RepoManifest::RepoManifest(QString fileDir) :
    QFile(fileDir + "/repo-manifest"),
    FileEditor(fileDir + "/repo-manifest")
{

}

void RepoManifest::updateDependency(QString dependencyName, QString oldTag, QString newTag)
{
    try
    {
        updateValueInLinesContainingKeyword(oldTag, newTag, dependencyName);
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}

void RepoManifest::updateDependency(QString dependencyName, QString newTag)
{

}

void RepoManifest::updateVersionTo(QString newVersion)
{
    try
    {
        QString oldVersion = getValueOfKey("VERSION", '=');
        updateValueInLinesContainingKeyword(oldVersion, newVersion, "VERSION");
    }
    catch (MyError e)
    {
        e.displayError();
        throw;
    }
}
