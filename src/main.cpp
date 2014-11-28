/**
 * @file main.cpp
 * @author Johan Simonsson
 * @brief Main for the Telldus Duo WeekTimer test program
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

//For the WeekTimer
#include <QDebug>
#include <QString>
#include <QDate>
#include <QTime>

//For sleep
#include <unistd.h>

// For the Telldus Duo
#include <telldus-core.h>
#include <mosquittopp.h>

#include "MosqConnect.h"
#include "UnixTime.h"
#include "WeekTimer.h"
#include "WeekTimerLine.h"


int main()
{
    qDebug() << "WeekTimer";
    tdInit();

    // This list comes from /etc/tellstick.conf
    // that was created with TelldusCenter.
    //
    // The name given in TelldusCenter also becomes the
    // uniq part of the mqtt topic.

    QList<WeekTimer> *weekTimerList;
    weekTimerList = new QList<WeekTimer>;

    int intNumberOfDevices = tdGetNumberOfDevices();
    for (int i = 0; i < intNumberOfDevices; i++)
    {
        int id = tdGetDeviceId( i );
        char *nameTmp = tdGetName( id );
        QString name = QString(nameTmp);

        int methods = tdMethods( id, (TELLSTICK_TURNON | TELLSTICK_TURNOFF) );
        if ( (methods & TELLSTICK_TURNON ) && (methods & TELLSTICK_TURNOFF) )
        {
            WeekTimer wt(name);
            wt.setID(id);
            weekTimerList->append(wt);
        }
        /// @todo Check for bell later, and

        qDebug() << id << name;
        tdReleaseString(nameTmp);
    }


    for (int z = 0; z < weekTimerList->size(); ++z)
    {
        WeekTimer wt = weekTimerList->at(z);
        qDebug() << "WeekTimer name:" << wt.getName() << "id:" << wt.getID();
    }

    class MosqConnect *mqtt;
    int rc;

    mosqpp::lib_init();

    mqtt = new MosqConnect(
            "FunTechHouse_WeekTimer_Nexa",
            "mosqhub",
            1883,
            weekTimerList
            );

    while(1)
    {
        //Check if there is new mess for me
        rc = mqtt->loop();
        if(rc)
        {
            mqtt->reconnect();
        }

        //Then every minute or so,
        //update the timer output
        if(UnixTime::get()%30==0)
        {
            //qDebug() << UnixTime::get();

            QDate nowDate = QDate::currentDate();
            int dow = nowDate.dayOfWeek();

            QTime nowTime = QTime::currentTime();
            int hour = nowTime.hour();
            int min = nowTime.minute();

            qDebug() << "Time:" << dow << hour << min;


            for (int z = 0; z < weekTimerList->size(); ++z)
            {
                WeekTimer wt = weekTimerList->at(z);
                //qDebug() << "Rum" << wt.getName() << wt.getID();

                int id = wt.getID();
                if(id != -1)
                {
                    int methods = tdMethods( id, (TELLSTICK_TURNON | TELLSTICK_TURNOFF) );
                    if ( (methods & TELLSTICK_TURNON ) && (methods & TELLSTICK_TURNOFF) )
                    {
                        if(wt.isON(dow, hour, min))
                        {
                            qDebug() << wt.getName() << "Turn ON  at:" << dow << hour << min;
                            tdTurnOn(id);
                            sleep(1);
                        }
                        else
                        {
                            qDebug() << wt.getName() << "Turn OFF at:" << dow << hour << min;
                            tdTurnOff(id);
                            sleep(1);
                        }
                    }
                }
            }
        }
    }

    mosqpp::lib_cleanup();

    return 0;
}
