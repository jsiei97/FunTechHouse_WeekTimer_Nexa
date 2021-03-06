/**
 * @file MosqConnect.h
 * @author Johan Simonsson
 * @brief Mosquitto interface
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

#ifndef  __MOSQCONNECT_H
#define  __MOSQCONNECT_H

#include <mosquittopp.h>
#include "WeekTimer.h"
#include "SQLiteWrapper.h"

class MosqConnect : public mosqpp::mosquittopp
{
    private:
        QList<WeekTimer> *list;
        SQLiteWrapper *dblite;

    public:
        MosqConnect(
                const char *id,
                const char *host,
                int port,
                QList<WeekTimer> *list,
                SQLiteWrapper *db);
        ~MosqConnect();

        void on_connect(int rc);
        void on_message(const struct mosquitto_message *message);
        void on_subscribe(int mid, int qos_count, const int *granted_qos);
        void pub(QString topic, QString subject);
};

#endif  // __MOSQCONNECT_H
