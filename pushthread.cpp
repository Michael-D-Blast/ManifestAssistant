#include "pushthread.h"
#include <QDebug>

PushThread::PushThread()
{
    this->dot = dot;
}

int PushThread::getResult() const
{
    return result;
}

void PushThread::setDot(Dot *dot)
{
    this->dot = dot;
}

void PushThread::run()
{
    int ret = 0;

    qDebug() << "Dummy push local commits to remote";

    result = ret;
}
