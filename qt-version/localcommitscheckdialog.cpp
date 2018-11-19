#include "localcommitscheckdialog.h"

static const QString LOCAL_COMMIT_DIR = "/tmp/components";

LocalCommitsCheckDialog::LocalCommitsCheckDialog(QWidget *parent) :
    QDialog(parent)
{
    prompt = new QLabel(this);
    btn = new QPushButton(this);
    vLayout = new QVBoxLayout(this);

    // TODO: Get the local commit directory by dynamicall method

    prompt->setText(QString("Please check the local commits in %1 before pushing them to remote repository").arg(LOCAL_COMMIT_DIR));
    btn->setText("NEXT");

    vLayout->addWidget(prompt);
    vLayout->addWidget(btn, 0, Qt::AlignHCenter);

    connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnClicked()));
}

LocalCommitsCheckDialog::~LocalCommitsCheckDialog()
{
    delete prompt;
    delete btn;
}

void LocalCommitsCheckDialog::btnClicked()
{
    accept();
}
