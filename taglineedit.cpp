#include "taglineedit.h"
#include <QDebug>

TagLineEdit::TagLineEdit(ComponentsList *components, QWidget *parent) :
    QLineEdit(parent)
{
    this->components = components;
}

void TagLineEdit::componentChanged(int index)
{
//    qDebug() << "componentChanged index: " << index;

    // The first index of combo box is NULL, so index
    if (index == 0) {
        setText("");
    } else {
        setText((*components)[index-1].getTag());
    }
}
