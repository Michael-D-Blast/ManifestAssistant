#include "dialog.h"
#include <QApplication>
#include "backendthread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    BackendThread backendThread;

    backendThread.start();

    w.show();

    return a.exec();
}
