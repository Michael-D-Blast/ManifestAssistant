#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "backendthread.h"
#include "dot.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "component.h"
#include "taglineedit.h"

// TODO: Add components dynamically
#define COMPONENTS_MAX_NUM 10

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void setComboxItems();

    Dot dot;

private slots:
    void comboBoxNIndexChanged();
    void oKClicked();

private:
    struct ComponentInputter {
        QComboBox *name;
        QLineEdit *branch;
        TagLineEdit *tag;
    };

    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *ok;
    struct ComponentInputter componentInputters[COMPONENTS_MAX_NUM];

    BackendThread backendThread;

    // TODO: After Refactoring the class Component, choose a more appropriate type hold as less information as possible.
    ComponentsList components;      // Store the components information, get them from dot.
};

#endif // DIALOG_H
