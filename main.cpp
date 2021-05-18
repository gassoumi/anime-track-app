#include <QCoreApplication>
#include "database.h"
#include "webui.h"
#include "mainwindow.h"
#include <QDebug>
#include "webserver.h"
#include "model.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<QVector<SearchResult>>();
    qRegisterMetaType<model::Anime>();
    qRegisterMetaType<model::Episode>();
    if(!database::openConnection())
        return EXIT_FAILURE; // return 1

    MainWindow app;
    WebServer *workerThread = new WebServer();
    workerThread->start();
    return a.exec();
}

