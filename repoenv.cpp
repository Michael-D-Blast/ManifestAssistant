#include "repoenv.h"
#include "myerror.h"
#include <QTextStream>
#include <QDebug>
#include <QStringList>

const QString MT_COMPONENT_OPTS_KEYWORD = "MT_COMPONENT_OPTS";

RepoEnv::RepoEnv(QString filePath)
{
    envFile = new QFile(filePath);
    envData = new QHash<QString, PackageSource>();

    if(!envFile->open(QIODevice::ReadOnly | QIODevice::Text))
        throw MyError(-1, "Failed to open " + filePath, __LINE__, __FUNCTION__);

    QTextStream in(envFile);

    while(!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith(MT_COMPONENT_OPTS_KEYWORD)) {
            parseComponentOpts(line);
        }
    }
}

bool RepoEnv::isPackage(QString component)
{
    return (envData->value(component) == PKG);
}

int RepoEnv::parseComponentOpts(QString componentOpts)
{
    int startIndex = componentOpts.indexOf('(') + 1;
    int endIndex = componentOpts.indexOf(')') - 1;
    componentOpts = componentOpts.mid(startIndex, endIndex - startIndex + 1);

    QStringList componentOptsList = componentOpts.split(' ', QString::SkipEmptyParts);

    QRegExp sep("[\[,=]");
    for (int i = 0; i < componentOptsList.size(); i++) {
        // Item is something like [GuiModel,ORIGIN]=src
        QString component = componentOptsList[i].section(sep, 0, 0, QString::SectionSkipEmpty);
        QString sourceInString = componentOptsList[i].section(sep, 2, 2, QString::SectionSkipEmpty);
        PackageSource source;
        if (sourceInString == "src")
            source = SRC;
        else if (sourceInString == "pkg")
            source = PKG;
        else
            return -1;

        envData->insert(component, source);
    }

    return 0;
}
