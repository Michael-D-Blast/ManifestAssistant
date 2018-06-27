#ifndef BACKENDTHREAD_H
#define BACKENDTHREAD_H

#include <QThread>

class BackendThread:public QThread
{
public:
    BackendThread();

protected:
    void run();

private:

};

#endif // BACKENDTHREAD_H
