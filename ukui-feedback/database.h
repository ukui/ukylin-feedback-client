#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include "feedback.h"



static bool createConnection()
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");

    if(!db.open()) return false;

    QSqlQuery query;
    query.exec(QString(
                    "create table clock (Hour QString, Music QString,Filename Qstring)")); //提示：主键不能相同

    return true;
}

\
#endif // DATABASE_H
