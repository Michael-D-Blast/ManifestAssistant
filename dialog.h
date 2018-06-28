#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "backendthread.h"
#include "dot.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    Dot dot;

private slots:
    void on_pushButton_4_clicked();

private:
    Ui::Dialog *ui;

    BackendThread backendThread;
};

#endif // DIALOG_H
