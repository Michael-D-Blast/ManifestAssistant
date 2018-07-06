#include "backendthread.h"
#include <QDebug>
#include <QDir>
#include "myerror.h"
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

extern QWaitCondition waitCondition;
extern QMutex complete;

BackendThread::BackendThread()
{
    result = 0;
}

int BackendThread::getResult() const
{
    return result;
}

void BackendThread::setDot(Dot *dot)
{
    this->dot = dot;
}

void BackendThread::run()
{
    int ret = 0;

    qDebug() << "Run backend thread";

    if ((ret = createTmpDir()) < 0) {
        qDebug() << "Failed to create tmp dir in thread";
        result = -1;
        return ;
    }

    // TODO: Change the return value type from void to int and check its return value here instead of using try/catch
    try {
        dot->updateLocalManifests();
    }
    catch (MyError e) {
        e.displayError();
        result = -1;
    }

    result = ret;   // Set the result of thread, the main dialog will get it later
}

int BackendThread::createTmpDir()
{
    // Creating a temp dir to do the following work: update manifest, commit, create tag and so on.

    QDir dir("/");

    // If the tmp dir already exists, might be created when this program was executed last time, remove it first.

    if (dir.exists(TMP_COMPONENT_DIR)) {
        qDebug() << TMP_COMPONENT_DIR << " exists";
        qDebug() << "Removing " << TMP_COMPONENT_DIR;

        QDir dirToRemove(TMP_COMPONENT_DIR);
        if (dirToRemove.removeRecursively()) {
            qDebug() << "Succeeded to remove " << TMP_COMPONENT_DIR;
        } else {
            qDebug() << "Failed to remove" << TMP_COMPONENT_DIR;
            return -1;
        }
    }

    qDebug() << "Creating " << TMP_COMPONENT_DIR;
    if (dir.mkdir(TMP_COMPONENT_DIR)) {
        qDebug() << "Succeeded to create " << TMP_COMPONENT_DIR;
    } else {
        qDebug() << "Failed to create " << TMP_COMPONENT_DIR;
        return -2;
    }

    qDebug() << "Creating " << TMP_COMPONENT_DIR << "/commit_messages/";
    if (dir.mkdir(TMP_COMPONENT_DIR + "/commit_messages")) {
        qDebug() << "Succeeded to create " << TMP_COMPONENT_DIR << "/commit_messages/";
    } else {
        qDebug() << "Failed to create " << TMP_COMPONENT_DIR << "/commit_messages/";
        return -2;
    }

    return 0;
}
