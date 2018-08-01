#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QDir>


class Workspace : public QDir
{
public:
    explicit Workspace(QString path);

    bool isReusable() const;

private:
    bool reusable;
};

#endif // WORKSPACE_H
