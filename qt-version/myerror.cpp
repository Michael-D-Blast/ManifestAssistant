#include "myerror.h"
#include <QDebug>

MyError::MyError()
{

}

MyError::MyError(int ret, QString msg = "unset", int line = -1, QString func = "unset")
{
    this->ret = ret;
    this->msg = msg;
    this->line = line;
    this->func = func;
}

void MyError::displayError() const
{
    qDebug() << "Ret: " << ret << "\tMsg: " << msg << "\tLine: " << line << "\tFunc: " << func;
}
