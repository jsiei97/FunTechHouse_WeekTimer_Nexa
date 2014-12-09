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

#include "gpio_RaspberryPi.h"
#include "SysfsGPIO.h"

int main()
{
    qDebug() << "WeekTimer";
    tdInit();
    SQLiteWrapper lite;

    // This list comes from /etc/tellstick.conf
    // that was created with TelldusCenter.
    //
    // The name given in TelldusCenter also becomes the
    // uniq part of the mqtt topic.

    QList<WeekTimer> *weekTimerList;
    weekTimerList = new QList<WeekTimer>;

    //Add Motor1, Pin11
    //Add Motor2, Pin13
    //Add Utebel, Pin15
    //Take this from a config file or args later...
    {
        QString name("Motor1");
        WeekTimer wt(name);
        wt.setID(GPIO_Pin11);

        QString timerdata = lite.getWeekTimer(name);
        if(!timerdata.isEmpty())
        {
            wt.addNewTimers(timerdata);
        }
        // lite.getForce(name)
        weekTimerList->append(wt);
    }
    {
        QString name("Motor2");
        WeekTimer wt(name);
        wt.setID(GPIO_Pin13);

        QString timerdata = lite.getWeekTimer(name);
        if(!timerdata.isEmpty())
        {
            wt.addNewTimers(timerdata);
        }
        // lite.getForce(name)
        weekTimerList->append(wt);
    }
    {
        QString name("Utebel");
        WeekTimer wt(name);
        wt.setID(GPIO_Pin15);

        QString timerdata = lite.getWeekTimer(name);
        if(!timerdata.isEmpty())
        {
            wt.addNewTimers(timerdata);
        }
        // lite.getForce(name)
        weekTimerList->append(wt);
    }


    SysfsGPIO gpio;
    for (int z = 0; z < weekTimerList->size(); ++z)
    {
        WeekTimer wt = weekTimerList->at(z);
        qDebug() << "WeekTimer name:" << wt.getName() << "id:" << wt.getID();

        if(!gpio.configureGPIO((GPIO_Pin)wt.getID(), GPIO_DIRECTION_OUTPUT))
        {
            //myErr() << "configure failed...";
            return false;
        }
    }

    class MosqConnect *mqtt;
    int rc;

    mosqpp::lib_init();

    mqtt = new MosqConnect(
            "FunTechHouse_WeekTimer_Nexa",
            "mosqhub",
            1883,
            weekTimerList,
            &lite
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
                    {

                        WeekTimerOut status = wt.isON(dow, hour, min);
                        switch ( status )
                        {
                            case WT_ON:
                                {
                                    qDebug() << wt.getName() << "Turn ON  at:" << dow << hour << min;
                                    gpio.writeGPIO((GPIO_Pin)id, GPIO_HIGH);
                                    //sleep(1);
                                }
                                break;
                            case WT_DISABLED:
                                //Do nothing!
                                qDebug() << wt.getName() << "Not active at:" << dow << hour << min;
                                gpio.writeGPIO((GPIO_Pin)id, GPIO_LOW);
                                break;
                            case WT_OFF: //Fall thru ok
                            default :
                                {
                                    qDebug() << wt.getName() << "Turn OFF at:" << dow << hour << min;
                                    gpio.writeGPIO((GPIO_Pin)id, GPIO_LOW);
                                    //sleep(1);
                                }
                                break;
                        }
                    }
                }
            }
        }
    }

    mosqpp::lib_cleanup();

    return 0;
}
