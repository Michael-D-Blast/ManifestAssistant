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
    result = dot->pushLocalCommits();
}
