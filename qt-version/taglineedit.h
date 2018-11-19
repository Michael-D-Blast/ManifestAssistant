#ifndef TAGLINEEDIT_H
#define TAGLINEEDIT_H

#include <QObject>
#include "component.h"
#include <QLineEdit>

class TagLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    TagLineEdit(ComponentsList *components, QWidget *parent = 0);

public slots:
    void componentChanged(int index);

private:
    ComponentsList *components;
};

#endif // TAGLINEEDIT_H
