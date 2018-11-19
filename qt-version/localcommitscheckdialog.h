#ifndef LOCALCOMMITSCHECKDIALOG_H
#define LOCALCOMMITSCHECKDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class LocalCommitsCheckDialog : public QDialog
{
    Q_OBJECT

public:
    LocalCommitsCheckDialog(QWidget *parent = 0);
    ~LocalCommitsCheckDialog();

private slots:
    void btnClicked();

private:
    QLabel *prompt;
    QPushButton *btn;
    QVBoxLayout *vLayout;
};

#endif // LOCALCOMMITSCHECKDIALOG_H
