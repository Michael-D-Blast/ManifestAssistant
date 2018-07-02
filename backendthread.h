#ifndef BACKENDTHREAD_H
#define BACKENDTHREAD_H

#include <QThread>
#include "dot.h"

class BackendThread:public QThread
{
public:
    BackendThread();

    // Set Methods
    void setDot(Dot *dotInput);

protected:
    void run();

private:
    Dot *dot;

    int createTmpDir();
};

#endif // BACKENDTHREAD_H
