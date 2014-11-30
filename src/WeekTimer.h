/**
 * @file WeekTimer.h
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

#ifndef  __WEEKTIMER_H
#define  __WEEKTIMER_H

#include <QString>
#include <QList>

#include "WeekTimerLine.h"

/**
 * WeekTimer Output state
 */
typedef enum WTO
{
    WT_OFF = 0, ///< The timer output is OFF
    WT_ON,      ///< The timer output is ON
    WT_DISABLED ///< This timer is not active, no timerlines.
} WeekTimerOut;

/**
 * WeekTimer force status
 */
typedef enum WTF
{
    WT_FORCE_OFF = 0, ///< The timer output is forced OFF
    WT_FORCE_ON,      ///< The timer output is forced ON
    WT_FORCE_AUTO     ///< No active force, timer is back in auto
} WeekTimerForce;

class WeekTimer
{
    private:
        QString name;
        int id;
        QList<WeekTimerLine> timers;

        WeekTimerForce force;
        unsigned int forceTime;

        bool checkForceTime();

    public:
        WeekTimer(QString name);

        bool addNewTimers(QString data);
        QString getTimerString();

        WeekTimerOut isON(int dow, int hour, int min);

        void addForce(WeekTimerForce force, unsigned int time);
        //QString getForceStatus();

        bool isName(QString name);
        QString getName();

        void setID(int id);
        int getID();
};

#endif  // __WEEKTIMER_H
