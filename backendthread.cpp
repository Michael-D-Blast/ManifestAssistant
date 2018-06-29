#include "backendthread.h"
#include <QDebug>

BackendThread::BackendThread()
{

}

void BackendThread::setDot(Dot *dotInput)
{
    dot = dotInput;
}

void BackendThread::run()
{
    qDebug() << "Run backend thread";

    dot->updateLocalManifests();
}
