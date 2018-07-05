#ifndef BACKENDTHREAD_H
#define BACKENDTHREAD_H

#include <QThread>
#include "dot.h"

class BackendThread:public QThread
{
public:
    BackendThread();

    // Get Methods
    int getResult() const;

    // Set Methods
    void setDot(Dot *dot);

protected:
    void run();

private:
    Dot *dot;

    int createTmpDir();

    int result;
};

#endif // BACKENDTHREAD_H
