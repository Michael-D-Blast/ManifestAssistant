#include "dialog.h"
#include "ui_dialog.h"

// TODO: Add components dynamically
#define COMPONENTS_MAX_NUM 3

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setComboxItems()
{
    ComponentsList allComponents = dot.getAllComponentsList();

    ui->cBoxName1->addItem("NULL");
    ui->cBoxName2->addItem("NULL");
    ui->cBoxName3->addItem("NULL");

    for (int i = 0; i < allComponents.size(); i++)
    {
        ui->cBoxName1->addItem(allComponents[i].getName());
        ui->cBoxName2->addItem(allComponents[i].getName());
        ui->cBoxName3->addItem(allComponents[i].getName());
    }

    ui->cBoxBranch1->addItem("NULL");
    ui->cBoxBranch2->addItem("NULL");
    ui->cBoxBranch3->addItem("NULL");

//    for (int i = 0; i < allComponents.size(); i++)
//    {
//        ui->cBoxBranch1->addItem(allComponents[i].getName());
//        ui->cBoxBranch2->addItem(allComponents[i].getName());
//        ui->cBoxBranch3->addItem(allComponents[i].getName());
//    }
}


void Dialog::on_BtnOK_clicked()
{
    backendThread.start();

    ui->BtnOK->setEnabled(false);
}
