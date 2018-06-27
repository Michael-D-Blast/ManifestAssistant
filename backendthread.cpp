#include "backendthread.h"
#include <QDebug>
#include "dot.h"

// TODO: Calulate the path according to the working dir environment
const QString RootDotFile = "/home/bsp/mtws/obj/armbuildroot/Esmeralda/repo.dot";

BackendThread::BackendThread()
{

}

void BackendThread::run()
{
    qDebug() << "Read the Esmeralda.dot" << endl;

    Dot dot(RootDotFile);

    // 1. Parse the dot file, get a list of the dependency pairs
    dot.parseDependencyTree();

    // 2. Split the components to serveral groups in diffent levels
    dot.generateDependencyPyramid();

    dot.displayDependencyPyramid();

}
