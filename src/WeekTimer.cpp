/**
 * @file WeekTimer.cpp
 * @author Johan Simonsson
 * @brief A WeekTimer
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

#include <QStringList>
#include <QString>
#include <QDebug>

#include "WeekTimer.h"
#include "WeekTimerLine.h"

WeekTimer::WeekTimer(QString name)
{
    this->name = name;
    id = -1;
}

/**
 * Replace all the timers
 *
 * Data in is the list with ; as separator.
 * So if I would like to add
 * Timer1: 8 06 00 - 8 09 00
 * Timer2: 8 17 00 - 8 23 00
 * Timer3: 9 08 00 - 9 01 00
 *
 * This would give a string like this:
 * "8:06:00-8:09:00;8:17:00-8:23:00;9:08:00-9:01:00"
 *
 * @param data with the new timers
 * @return true if ok
 */
bool WeekTimer::addNewTimers(QString data)
{
    QStringList list = data.split(";", QString::SkipEmptyParts);

    bool res = true;

    timers.clear();

    WeekTimerLine wtl;
    for (int i = 0; i < list.size(); ++i)
    {
        qDebug() << list.at(i);
        if(wtl.setLine(list.at(i)))
        {
            timers.append(wtl);
        }
        else
        {
            qDebug() << "Error: can add" << list.at(i);
            res = false;
        }
    }
    return res;
}

QString WeekTimer::getTimerString()
{
    QString str;
    for( int i=0 ; i<timers.size() ; i++ )
    {
        if(i!=0)
        {
            str.append(";");
        }
        WeekTimerLine wtl = timers.at(i);
        str.append(wtl.getLine());
    }
    return str;
}

WeekTimerOut WeekTimer::isON(int dow, int hour, int min)
{
    WeekTimerOut turnON = WT_OFF;

    if(timers.empty())
    {
        return WT_DISABLED;
    }

    for( int i=0 ; i<timers.size() ; i++ )
    {
        WeekTimerLine wtl = timers.at(i);
        if(wtl.isON(dow, hour, min))
        {
            turnON = WT_ON;
        }
    }
    return turnON;
}

bool WeekTimer::isName(QString name)
{
    return (this->name.compare(name) == 0);
}

QString WeekTimer::getName()
{
    return name;
}

void WeekTimer::setID(int id)
{
    this->id=id;
}

int WeekTimer::getID()
{
    return id;
}

