#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include <QWaitCondition>
#include <QMutex>
#include <QFileDialog>
#include "localcommitscheckdialog.h"

// TODO: Calulate the path according to the working dir environment

#define MY_DEBUG

QWaitCondition waitCondition;
QMutex complete;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

#ifdef MY_DEBUG
    w.dot.workingDir = "/home/bsp/mtws";
    QString dotFile = "/home/bsp/mtws/obj/armbuildroot/Esmeralda/repo.dot";
#else
    w.dot.workingDir = QFileDialog::getExistingDirectory(0, "Please choose your working directory",
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    QString dotFile = QFileDialog::getOpenFileName(0, "Please choose repo.dot", w.dot.workingDir + "/obj");
#endif
    w.dot.setFile(dotFile);

    // 1. Parse the dot file, get a list of the dependency pairs
    w.dot.parseDependencyTree();

    // 2. Split the components to serveral groups in diffent levels
    w.dot.generateDependencyPyramid();

    w.dot.displayDependencyPyramid();

    w.dot.generateAllComponentsList();

    w.setComboxItems();

    w.show();

    return a.exec();
}
