#ifndef COMPONENTSRCDIR_H
#define COMPONENTSRCDIR_H

#include "componentdir.h"

class ComponentSrcDir : public ComponentDir
{
public:
    explicit ComponentSrcDir(QString workspacePath, QString componentName, QString componentTag);

    bool canMakePkg() const;

    void init();
    void makePackage();
};

#endif // COMPONENTSRCDIR_H
