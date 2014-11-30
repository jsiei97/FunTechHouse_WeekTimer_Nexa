/**
 * @file SQLiteWrapper.h
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

#ifndef  __SQLITEWRAPPER_H
#define  __SQLITEWRAPPER_H

#include <QSqlDatabase>
#include <QMutex>

class SQLiteWrapper
{
    private:
        QString filename;
        QSqlDatabase db;
        QMutex mutex;

        void open();
        void close();

    public:
        SQLiteWrapper();
        //SQLiteWrapper()~;

        //QString getId(QString topic);
        //void updateTimestamp(QString topic);
        //QStringList getOldTopics();
};

#endif  // __SQLITEWRAPPER_H
