#ifndef COMPONENTPKGDIR_H
#define COMPONENTPKGDIR_H

#include "componentdir.h"

class ComponentPkgDir : public ComponentDir
{
public:
    explicit ComponentPkgDir(QString workspacePath, QString componentName, QString componentTag);

    void init();
    void makePackage();
};

#endif // COMPONENTPKGDIR_H
