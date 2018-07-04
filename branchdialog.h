#ifndef BRANCHDIALOG_H
#define BRANCHDIALOG_H

#include <QObject>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

class BranchDialog : public QDialog
{
    Q_OBJECT

public:
    BranchDialog(QString component, QWidget *parent = 0);
    ~BranchDialog();

    QString getBranch() const;

private slots:
    void saveBranch();

private:
    QVBoxLayout *vLayout;
    QLabel *instruction;

    // TODO: Use a combo box display all the branches that this component has

    QLineEdit *lineedit;
    QPushButton *ok;

    QString branch;
};

#endif // BRANCHDIALOG_H
