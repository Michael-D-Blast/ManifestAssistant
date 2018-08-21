#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include <QWaitCondition>
#include <QMutex>
#include <QFileDialog>
#include "localcommitscheckdialog.h"
#include "repoenv.h"
#include "version.h"

// TODO: Calulate the path according to the working dir environment

//#define MY_DEBUG

QWaitCondition waitCondition;
QMutex complete;

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        QString arg(argv[i]);
//        qDebug() << arg;

        if (arg == "-V") {
            qDebug() << "Version: " + QString(version);
            return 0;
        } else {
            qDebug() << "Unknown arguments";
            return -1;
        }
    }

    QApplication a(argc, argv);
    Dialog w;

#ifdef MY_DEBUG
    w.dot.workingDir = "/home/abb/mtws";
    QString dotFile = "/home/abb/mtws/obj/armbuildroot/Esmeralda/repo.dot";
    QString envFile = "/home/abb/mtws/obj/armbuildroot/Esmeralda/repo.env";
#else
    w.dot.workingDir = QFileDialog::getExistingDirectory(0, "Please choose your working directory",
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if (w.dot.workingDir.isEmpty()) {
        qDebug() << "Working directory is not specified!";
        return(1);
    }

    QString dotFile = QFileDialog::getOpenFileName(0, "Please choose repo.dot", w.dot.workingDir + "/obj");
    QString envFile = QFileDialog::getOpenFileName(0, "Please choose repo.env", w.dot.workingDir + "/obj");
#endif

    RepoEnv repoEnv(envFile);

    w.dot.setRepoEnv(&repoEnv);
    w.dot.setFile(dotFile);
    w.dot.parseDependencyTree();
    w.dot.generateDependencyPyramid();
    w.dot.generatePackagesWithoutSourceCode();
//    w.dot.displayPackagesWithoutSourceCode();
//    w.dot.fetchSourceCodesOfPackages();
    w.dot.setDependenciesForPyramid();
//    w.dot.displayDependencyPyramid();

    w.dot.generateAllComponentsList();

    w.setComboxItems();

    w.show();

    return a.exec();
}
