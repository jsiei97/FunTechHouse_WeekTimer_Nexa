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

// For the Telldus Duo
#include <telldus-core.h>

#include "WeekTimer.h"
#include "WeekTimerLine.h"


int main(int argc, char *argv[])
{
    qDebug() << "WeekTimer";
    tdInit();

    WeekTimer rum1("Rum1");
    WeekTimer rum2("Rum2");
    WeekTimer rum3("Rum3");


    QString timeData;

    qDebug() << "Rum3 add data:";
    timeData.clear();
    //Mon..Fri
    timeData.append("8:06:00-8:10:00;");
    timeData.append("8:15:30-8:22:00;");
    //Sat, Sun
    timeData.append("9:08:00-9:12:00;");
    timeData.append("6:12:00-6:23:00;");
    timeData.append("7:12:00-7:22:00;");
    rum3.addNewTimers(timeData);

    qDebug() << "Rum2 add data:";
    timeData.clear();
    //Mon..Fri
    timeData.append("8:06:00-8:10:00;");
    timeData.append("8:15:30-8:23:00;");
    //Sat, Sun
    timeData.append("9:08:00-9:23:00;");
    rum2.addNewTimers(timeData);


    qDebug() << "Rum1 add data:";
    timeData.clear();
    timeData.append("0:17:00-0:10:00;");
    rum1.addNewTimers(timeData);

    QList<WeekTimer> weekTimerList;
    weekTimerList.append(rum1);
    weekTimerList.append(rum2);
    weekTimerList.append(rum3);


    int intNumberOfDevices = tdGetNumberOfDevices();
    for (int i = 0; i < intNumberOfDevices; i++)
    {
        int id = tdGetDeviceId( i );
        char *nameTmp = tdGetName( id );
        QString name = QString(nameTmp);

        for (int z = 0; z < weekTimerList.size(); ++z)
        {
            WeekTimer wt = weekTimerList.at(z);
            if(wt.isName(name))
            {
                //qDebug() << id << name;
                wt.setID(id);
                weekTimerList.replace(z, wt);
            }
        }

        //printf("%d\t%s\n", id, name);
        qDebug() << id << name;
        tdReleaseString(nameTmp);
    }

    for (int z = 0; z < weekTimerList.size(); ++z)
    {
        WeekTimer wt = weekTimerList.at(z);
        qDebug() << "Rum" << wt.getName() << wt.getID();
    }


    //for( int k=0 ; k<3 ; k++ )
    while(true)
    {
        QDate nowDate = QDate::currentDate();
        int dow = nowDate.dayOfWeek();

        QTime nowTime = QTime::currentTime();
        int hour = nowTime.hour();
        int min = nowTime.minute();

        qDebug() << "Time:" << dow << hour << min;


        for (int z = 0; z < weekTimerList.size(); ++z)
        {
            WeekTimer wt = weekTimerList.at(z);
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
        sleep(1*60);
    }

    return 0;
}
