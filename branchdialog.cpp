#include "branchdialog.h"
#include <QMessageBox>
#include <QDebug>

BranchDialog::BranchDialog(QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
    instruction = new QLabel(this);
    lineedit = new QLineEdit(this);
    ok = new QPushButton(this);

    branch = "";

    instruction->setText("Please specify to which branch to commit");

    ok->setText("OK");

    vLayout->addWidget(instruction);
    vLayout->addWidget(lineedit);
    vLayout->addWidget(ok);

    setLayout(vLayout);

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(saveBranch()));
}

BranchDialog::~BranchDialog()
{
    delete vLayout;
    delete instruction;
    delete lineedit;
    delete ok;
}

QString BranchDialog::getBranch() const
{
    return branch;
}

void BranchDialog::saveBranch()
{
    branch = lineedit->text();
    if (branch.isEmpty()) {
        QMessageBox::warning(this, "WARN", "Branch cannot be empty!", QMessageBox::Yes);
        return;
    }

    accept();
}
