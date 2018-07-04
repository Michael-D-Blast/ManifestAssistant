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
    void oKClicked();
    void responseBranchDialog();

private:
    struct ComponentInputter {
        QComboBox *name;

        // TODO: Actually, we don't need the branch here, because user don't need to specify branch here

        QLineEdit *branch;
        TagLineEdit *tag;
    };

    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *componentName;
    QLabel *componentBranch;
    QLabel *componentTag;

    QPushButton *ok;
    struct ComponentInputter componentInputters[COMPONENTS_MAX_NUM];

    BackendThread backendThread;

    // TODO: After Refactoring the class Component, choose a more appropriate type hold as less information as possible.
    ComponentsList components;      // Store the components information, get them from dot.

    // Check the input branch, tag of the component to be updated is valid
    bool componentInputIsValid(Component);
};

#endif // DIALOG_H
