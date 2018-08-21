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

static const char DEFAULT_ROOT_COMPONENT[] = "Esmeralda";
static const char DEFAULT_PLATFORM[] = "armbuildroot";
//#define MY_DEBUG

QWaitCondition waitCondition;
QMutex complete;

int main(int argc, char *argv[])
{
    QString rootComponent(DEFAULT_ROOT_COMPONENT);
    QString platform(DEFAULT_PLATFORM);

    for (int i = 1; i < argc; ++i) {
        QString arg(argv[i]);
//        qDebug() << arg;

        // TODO: Add argument to support specifying root component and platform
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
#else
    w.dot.workingDir = QFileDialog::getExistingDirectory(0, "Please choose your working directory",
                                                     getenv("HOME"),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

#endif
    if (w.dot.workingDir.isEmpty()) {
        qDebug() << "Working directory is not specified!";
        return(-1);
    }

    QString dotFile = w.dot.workingDir + "/obj/" + platform + "/" + rootComponent + "/repo.dot";
    QFile fileDot(dotFile);
    if (!fileDot.exists()) {
        qDebug() << dotFile + " doesn't exist!";
        return(-1);
    }
    qDebug() << "dotFile is " << dotFile;

    QString envFile = w.dot.workingDir + "/obj/" + platform + "/" + rootComponent + "/repo.env";
    QFile fileEnv(envFile);
    if (!fileEnv.exists()) {
        qDebug() << envFile + " doesn't exist!";
        return(-1);
    }
    qDebug() << "envFile is " << envFile;

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
