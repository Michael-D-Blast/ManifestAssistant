#include "workspace.h"

Workspace::Workspace(QString path) :
    QDir(path)
{

}

bool Workspace::isReusable() const
{
    return reusable;
}
