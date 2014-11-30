/**
 * @file SQLiteWrapper.cpp
 * @author Johan Simonsson
 * @brief A SQLite wrapper
 */

/*
 * Copyright (C) 2014 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>

#include "SQLiteWrapper.h"
#include "UnixTime.h"

void SQLiteWrapper::open()
{
    mutex.lock();
    //Please note that if there is no file, it will be created!
    if( !db.open() )
    {
        qFatal("Failed to open db file!");
    }
}

void SQLiteWrapper::close()
{
    db.close();
    mutex.unlock();
}

SQLiteWrapper::SQLiteWrapper()
{
    //qDebug() << "Qt SQLite test app";
    filename = "weektimer.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);

    open();

    QStringList tableList = db.tables(QSql::Tables);
    if( tableList.size() == 0 )
    {
        qDebug() << "Warning empty db";

        QSqlQuery query;
        // name and timerstring
        if( !query.exec("CREATE TABLE IF NOT EXISTS WeekTimer \
                    ( \
                      name      VARCHAR(256), \
                      timerdata VARCHAR(2048) \
                    )")
          )
        {
            qFatal("Failed to create table, WeekTimer!");
        }

        if( !query.exec("CREATE TABLE IF NOT EXISTS Force \
                    ( \
                      name  VARCHAR(256), \
                      state VARCHAR(10), \
                      time  INTEGER \
                    )")
          )
        {
            qFatal("Failed to create table, Force!");
        }
    }

    close();
};

/*
QString SQLiteWrapper::getId(QString topic)
{

    //find id
    unsigned int id = 0;
    bool ok = false;

    QString str("SELECT id FROM data WHERE topic like '");
    str.append(topic);
    str.append("' LIMIT 1");

    open();

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //qDebug() << "Number of columns: " << rec.count();

    int nameCol = rec.indexOf("id"); // index of the field "name"
    //if size==1 ...
    while (q.next())
    {
        id = q.value(nameCol).toUInt(&ok);
        if(ok)
        {
            break;
        }
        //qDebug() << q.value(nameCol).toString(); // output all names
    }

    close();

    if(ok)
    {
        QString idS;
        idS.setNum(id);
        str.append(idS);
        return idS;
    }
    else
    {
        return NULL;
    }
}

void SQLiteWrapper::updateTimestamp(QString topic)
{
    QString id = getId(topic);

    open();

    if(id == NULL)
    {
        QString str("INSERT INTO data ( topic, time ) VALUES ('");
        str.append(topic);
        str.append("', '");
        str.append(UnixTime::toQString());
        str.append("')");
        //qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            qFatal("Failed to insert");
        }
    }
    else
    {
        //update:
        //set current time
        //reset alarm values
        QString str("UPDATE data SET alarm=0, time='");
        str.append(UnixTime::toQString());
        str.append("' WHERE id='");
        str.append(id);
        str.append("'");
        //qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            qFatal("Failed to update");
        }
    }

    close();
}

QStringList SQLiteWrapper::getOldTopics()
{
    QStringList list;

    unsigned int alarmLimit = 2*60*60; ///< Data older that Xs will trigger a alarm,
    unsigned int now  = UnixTime::get();
    unsigned int past = now-alarmLimit;

    //qDebug() << now;
    //qDebug() << past;

    QString str("SELECT id, topic FROM data WHERE time < ");
    {
        QString strPast;
        strPast.setNum(past);
        str.append(strPast);
    }
    str.append(" AND alarm=0 ORDER BY topic");
    //qDebug() << str;

    open();

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //qDebug() << "Number of columns: " << rec.count();

    int colId = rec.indexOf("id"); // index of the field "name"
    int colTopic = rec.indexOf("topic"); // index of the field "name"
    while (q.next())
    {
        //qDebug() << q.value(colId).toString() << q.value(colTopic).toString();
        qDebug() << "Alarm data is old:" << alarmLimit << q.value(colTopic).toString();

        //id -> update alarm=1
        //topic -> list -> return

        QString str2("UPDATE data SET alarm=1 WHERE id='");
        str2.append(q.value(colId).toString());
        str2.append("'");
        //qDebug() << str2;

        QSqlQuery query;
        if( !query.exec(str2) )
        {
            qDebug() << "Error" << str2;
            qFatal("Failed to update");
        }
        list << q.value(colTopic).toString();
    }

    close();

    return list;
}
*/
