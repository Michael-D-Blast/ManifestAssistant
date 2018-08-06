#ifndef REPOEXECUTOR_H
#define REPOEXECUTOR_H

#include <QStringList>
#include "component.h"

class RepoExecutor
{
public:
    RepoExecutor();

    void select(QString dir = "");
    ComponentsList getList(QString dir = "");
};

#endif // REPOEXECUTOR_H
