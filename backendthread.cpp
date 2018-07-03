#include "backendthread.h"
#include <QDebug>
#include <QDir>
#include "myerror.h"

BackendThread::BackendThread()
{

}

void BackendThread::setDot(Dot *dotInput)
{
    dot = dotInput;
}

void BackendThread::run()
{
    int ret;

    qDebug() << "Run backend thread";

    if ((ret = createTmpDir()) < 0) {
        qDebug() << "Failed to create tmp dir in thread";
    }

    try {
        dot->updateLocalManifests();
    }
    catch (MyError e) {
        e.displayError();
    }
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

    return 0;
}
