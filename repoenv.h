#ifndef REPOENV_H
#define REPOENV_H

#include <QFile>
#include <QHash>

enum PackageSource {
    SRC,
    PKG,
};

class RepoEnv
{
public:
    explicit RepoEnv(QString filePath);

    bool isPackage(QString component);

private:
    QFile *envFile;
    QHash<QString, PackageSource> *envData;

    int parseComponentOpts(QString componentOpts);
};

#endif // REPOENV_H
