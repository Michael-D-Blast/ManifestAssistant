#ifndef MYERROR_H
#define MYERROR_H

#include <QString>

class MyError
{
public:
    MyError();
    MyError(int ret, QString msg, int line, QString func);

    void displayError() const;

private:
    int ret;
    QString msg;
    int line;
    QString func;

};

#endif // MYERROR_H
