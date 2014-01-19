/**
 * @file WeekTimerLine.cpp
 * @author Johan Simonsson
 * @brief A line of data for a WeekTimer
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
#include <QString>
#include <QRegExp>

#include "WeekTimerLine.h"


WeekTimerLine::WeekTimerLine()
{
    t1_dow = -1;
    t1_h = 0;
    t1_m = 0;

    t2_dow = 0;
    t2_h = 0;
    t2_m = 0;
}

// 8:06:00-8:09:00
bool WeekTimerLine::setLine(QString line)
{
    QRegExp rx("([0-9]{1,1}):([0-9]{1,2}):([0-9]{1,2})-([0-9]{1,1}):([0-9]{1,2}):([0-9]{1,2})");
    int cnt = rx.indexIn(line);

    if(cnt == -1 || rx.captureCount() != 6)
    {
        t1_dow = -1;
        return false;
    }

    //for( int i=1 ; i<=6 ; i++ )
    //{
    //qDebug() << rx.cap(i);
    //}

    bool ok = false;
    bool res = true;

    t1_dow = rx.cap(1).toInt(&ok, 10);
    if(!ok)
        res = false;

    t1_h = rx.cap(2).toInt(&ok, 10);
    if(!ok)
        res = false;

    t1_m = rx.cap(3).toInt(&ok, 10);
    if(!ok)
        res = false;

    t2_dow = rx.cap(4).toInt(&ok, 10);
    if(!ok)
        res = false;

    t2_h = rx.cap(5).toInt(&ok, 10);
    if(!ok)
        res = false;

    t2_m = rx.cap(6).toInt(&ok, 10);
    if(!ok)
        res = false;

    /// @todo Add some sanitychecks so the data is within range.

    return res;
}

int WeekTimerLine::timeSinceWeekStart(int d, int h, int m)
{
    int time = 0;

    time += d*24*60;
    time += h*60;
    time += m;

    return time;
}

/**
 * Is line on or off for this time?
 *
 * Args in is current time
 *
 * @param day current day of week, 1 is mon, 7 is sun
 * @param h current hour of the day, 24h clock
 * @param m current minute of the hour, 00..59
 * @return true for on, false for off
 */
bool WeekTimerLine::isON(int day, int hour, int min)
{
    if(-1 == t1_dow)
        return false;


    //If there is no wildcards
    if( ( t1_dow != 0 && t1_dow <= 7 ) && ( t2_dow != 0 && t2_dow <= 7 ) )
    {
        int t1  = timeSinceWeekStart( t1_dow, t1_h, t1_m );
        int t2  = timeSinceWeekStart( t2_dow, t2_h, t2_m );
        int now = timeSinceWeekStart( day,    hour, min );

        if(t1<t2)
        {
            if (t1 <= now  && now < t2)
            {
                //qDebug() << t1;
                //qDebug() << now << day << hour << min;
                //qDebug() << t2;
                return true;
            }
        }
        else
        {
            if (now >= t1 || now < t2)
            {
                //qDebug() << t1;
                //qDebug() << now << day << hour << min;
                //qDebug() << t2;
                return true;
            }

        }
        return false;
    }

    //dow 0, all days
    if( ( t1_dow == 0 ) && ( t2_dow == 0 ) )
    {
        int t1  = timeSinceWeekStart( 0, t1_h, t1_m );
        int t2  = timeSinceWeekStart( 0, t2_h, t2_m );
        int now = timeSinceWeekStart( 0, hour, min );

        if(t1<t2)
        {
            if (t1 <= now  && now < t2)
            {
                return true;
            }
        }
        else
        {
            if (now >= t1 || now < t2)
            {
                return true;
            }

        }
        return false;
    }

    //dow 8, all weekday (and sometimes a little bit into the day after).
    if( ( t1_dow == 8 ) && ( t2_dow == 8 ) )
    {
        int t1  = timeSinceWeekStart( 0, t1_h, t1_m );
        int t2  = timeSinceWeekStart( 0, t2_h, t2_m );
        int now = timeSinceWeekStart( 0, hour, min );

        if(t1<t2)
        {
            if(day <= 5)
            {
                if (t1 <= now  && now < t2)
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            if(day == 1)
            {
                if (now >= t1)
                {
                    return true;
                }
            }
            else if(day <= 5)
            {
                if (now >= t1 || now < t2)
                {
                    return true;
                }
            }
            else if(day == 6)
            {
                if (now < t2)
                {
                    return true;
                }
            }
            return false;
        }
    }

    //dow 9, all weekends (and sometimes a little bit into the day after).
    if( ( t1_dow == 9 ) && ( t2_dow == 9 ) )
    {
        int t1  = timeSinceWeekStart( 0, t1_h, t1_m );
        int t2  = timeSinceWeekStart( 0, t2_h, t2_m );
        int now = timeSinceWeekStart( 0, hour, min );

        if(t1<t2)
        {
            if(day >= 6)
            {
                if (t1 <= now  && now < t2)
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            if(day >= 6)
            {
                if (now >= t1 || now < t2)
                {
                    return true;
                }
            }
            else if(day == 1)
            {
                if (now < t2)
                {
                    return true;
                }
            }
            return false;
        }
    }

    return false;
}
