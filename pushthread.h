#ifndef PUSHTHREAD_H
#define PUSHTHREAD_H

#include <QObject>
#include <QThread>
#include "dot.h"

class PushThread : public QThread
{
public:
    PushThread();

    int getResult() const;

    void setDot(Dot *dot);

protected:
    virtual void run();

private:
    Dot *dot;
    int result;
};

#endif // PUSHTHREAD_H
