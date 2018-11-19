#include "branchdialog.h"
#include <QMessageBox>
#include <QDebug>
#include "gitexecutor.h"

BranchDialog::BranchDialog(QString component, QWidget *parent) :
    QDialog(parent)
{
    vLayout = new QVBoxLayout(this);
    instruction = new QLabel(this);
    combo = new QComboBox(this);
    ok = new QPushButton(this);

    branch = "";

    instruction->setText("Please specify to which branch to commit for " + component);

    GitExecutor git;
    // TODO: Pass the address instead of using a fixed one
    QStringList items;
    items = git.getBranches("/tmp/components/" + component);
    combo->addItems(items);

    ok->setText("OK");

    vLayout->addWidget(instruction);
    vLayout->addWidget(combo, 0, Qt::AlignHCenter);
    vLayout->addWidget(ok, 0, Qt::AlignHCenter);

    setLayout(vLayout);

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(saveBranch()));
}

BranchDialog::~BranchDialog()
{
    delete vLayout;
    delete instruction;
    delete ok;
}

QString BranchDialog::getBranch() const
{
    return branch;
}

void BranchDialog::saveBranch()
{
    branch = combo->currentText();
    if (branch.isEmpty()) {
        QMessageBox::warning(this, "WARN", "Branch cannot be empty!", QMessageBox::Yes);
        return;
    }

    accept();
}
