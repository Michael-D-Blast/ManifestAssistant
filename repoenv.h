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

private:
    QFile *envFile;
    QHash<QString, PackageSource> *envData;

    int ParseComponentOpts(QString componentOpts);
};

#endif // REPOENV_H
