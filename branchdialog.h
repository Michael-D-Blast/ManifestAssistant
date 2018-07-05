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
#include <QComboBox>

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
    QComboBox *combo;
    QPushButton *ok;

    QString branch;
};

#endif // BRANCHDIALOG_H
