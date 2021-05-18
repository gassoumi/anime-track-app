#include "database.h"
#include<QSqlDatabase>
#include <QDebug>
#include<QSqlQuery>
#include <QSqlError>

bool database::openConnection()
{
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "first");
    db.setDatabaseName("anime.db");

    if (!db.open()) {
        qWarning() << "Unable to establish a database connection.\n"
                   "This example needs SQLite support. Please read "
                   "the Qt SQL driver documentation for information how "
                   "to build it.\n\n"
                   "Click Cancel to exit." ;

        return false;
    }

    QSqlQuery query(db);

    query.exec("PRAGMA foreign_keys = ON");

    /*
    QString sql = "CREATE TABLE IF NOT EXISTS anime (name varchar(255) primary key, "
               "searchKey varchar(255) NOT NULL, "
               "dateAjout Real DEFAULT (datetime('now', 'localtime')))";
               */

    query.exec("CREATE TABLE IF NOT EXISTS anime (name varchar(255) primary key, "
               "searchKey varchar(255) NOT NULL, "
               "dateAjout Real DEFAULT CURRENT_TIMESTAMP)");

    query.exec("CREATE TABLE IF NOT EXISTS episode (fileUrl varchar(255) primary key, "
               "fileName varchar(255) NOT NULL, "
               "fileSize Real ,"
               "nbSeeders Real ,"
               "nbLeechers Real ,"
               "siteUrl varchar(255) ,"
               "descrLink varchar(255) ,"
               "number INTEGER NOT NULL ,"
               "name varchar(255) NOT NULL ,"
               "CONSTRAINT UC_Episode UNIQUE (number,name),"
               "FOREIGN KEY (name) REFERENCES anime (name) on delete cascade )");
    qDebug() << query.lastError();

    return true;
}
