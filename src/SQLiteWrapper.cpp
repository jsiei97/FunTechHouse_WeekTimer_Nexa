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

void SQLiteWrapper::init()
{
    qDebug() << "SQLite init:" << filename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);

    open();

    QStringList tableList = db.tables(QSql::Tables);
    if( tableList.size() == 0 )
    {
        //qDebug() << "Warning empty db";

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
}

SQLiteWrapper::SQLiteWrapper()
{
    //filename = "weektimer.db";
    filename = "/var/db/WeekTimerGPIO.db";
    init();
};

SQLiteWrapper::SQLiteWrapper(QString file)
{
    filename = file;
    init();
};

/**
 * Check if name exists in table WeekTimer
 *
 * @param name table row name
 * @return true if data exist, false if missing
 */
bool SQLiteWrapper::checkWeekTimerName(QString name)
{
    QString str("SELECT name FROM WeekTimer WHERE name LIKE '");
    str.append(name);
    str.append("' LIMIT 1");

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //int rows = rec.count();
    int rows = 0;
    //int nameCol = rec.indexOf("name"); // index of the field "name"
    while (q.next())
    {
        rows++;
        //qDebug() << q.value(nameCol).toString(); // output all names
    }
    //qDebug() << "Number of columns: " << rows;

    if(1 == rows)
    {
        return true;
    }
    return false;
}

/**
 * Check if name exists in table Force
 *
 * @param name table row name
 * @return true if data exist, false if missing
 */
bool SQLiteWrapper::checkForceName(QString name)
{
    QString str("SELECT name FROM Force WHERE name LIKE '");
    str.append(name);
    str.append("' LIMIT 1");

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //int rows = rec.count();
    int rows = 0;
    //int nameCol = rec.indexOf("name"); // index of the field "name"
    while (q.next())
    {
        rows++;
        //qDebug() << q.value(nameCol).toString(); // output all names
    }
    //qDebug() << "Number of columns: " << rows;

    if(1 == rows)
    {
        return true;
    }
    return false;
}

bool SQLiteWrapper::updateWeekTimer(QString name, QString timerdata)
{
    open();
    bool res = true;
    if(checkWeekTimerName(name))
    {
        //Update:
        QString str("UPDATE WeekTimer SET timerdata='");
        str.append(timerdata);
        str.append("' WHERE name='");
        str.append(name);
        str.append("'");
        //qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to update");
        }
    }
    else
    {
        //Insert:
        //INSERT INTO WeekTimer (name, timerdata) VALUES ('r1', '4:00:00-5:00:00');
        QString str("INSERT INTO WeekTimer ( name, timerdata ) VALUES ('");
        str.append(name);
        str.append("', '");
        str.append(timerdata);
        str.append("')");
        //qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to insert");
        }
    }
    close();
    return res;
}

bool SQLiteWrapper::updateForce(QString name, QString state, unsigned int time)
{
    bool res = true;
    open();

    //First do some house cleaning, remove outdated forced overides.
    //DELETE FROM Force WHERE time < '"' and time != 0;
    {
        QString str("DELETE FROM Force WHERE time < '");
        str.append(QString("%1").arg(UnixTime::get()));
        str.append("' AND time != 0");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to update");
        }
    }


    QString timestamp;
    if(0==time)
    {
        timestamp.append(QString("%1").arg(time));
    }
    else
    {
        time=UnixTime::get()+(time*60);
        timestamp.append(QString("%1").arg(time));
    }
    //qDebug() << timestamp << time;

    //Then since AUTO means no force, this also removes any active rows.
    //DELETE FROM Force WHERE name LIKE 'r2';
    if(state.compare("AUTO")==0)
    {
        QString str("DELETE FROM Force WHERE name LIKE '");
        str.append(name);
        str.append("'");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to update");
        }
    }
    else if(checkForceName(name))
    {
        //Update:
        QString str("UPDATE Force SET state='");
        str.append(state);
        str.append("', time='");
        str.append(timestamp);
        str.append("' WHERE name='");
        str.append(name);
        str.append("'");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to update");
        }
    }
    else
    {
        //Insert:
        //INSERT INTO WeekTimer (name, timerdata) VALUES ('r1', '4:00:00-5:00:00');
        QString str("INSERT INTO Force ( name, state, time ) VALUES ('");
        str.append(name);
        str.append("', '");
        str.append(state);
        str.append("', '");
        str.append(timestamp);
        str.append("')");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            res = false;
            //qFatal("Failed to insert");
        }
    }
    close();
    return res;
}

QString SQLiteWrapper::getWeekTimer(QString name)
{
    QString timerdata;

    QString str("SELECT timerdata FROM WeekTimer WHERE name LIKE '");
    str.append(name);
    str.append("' LIMIT 1");
    //qDebug() << str;

    open();

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //qDebug() << "Number of columns: " << rec.count();

    int colTimerData = rec.indexOf("timerdata");
    while (q.next())
    {
        //qDebug() << q.value(colTimerData).toString();
        timerdata = q.value(colTimerData).toString();
    }

    close();
    return timerdata;
}

/*

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
