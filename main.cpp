#include "dialog.h"
#include <QApplication>

// TODO: Calulate the path according to the working dir environment
const QString RootDotFile = "/home/bsp/mtws/obj/armbuildroot/Esmeralda/repo.dot";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    w.dot.setFile(RootDotFile);

    // 1. Parse the dot file, get a list of the dependency pairs
    w.dot.parseDependencyTree();

    // 2. Split the components to serveral groups in diffent levels
    w.dot.generateDependencyPyramid();

    w.dot.displayDependencyPyramid();

    w.show();

    return a.exec();
}
