/**
 * @file WeekTimerLine.h
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

#ifndef  __WEEKTIMERLINE_H
#define  __WEEKTIMERLINE_H

#include <QString>

/**
 * A line in a WeekTimer
 *
 * dow hh mm - dow hh mm
 * dow    0 all days
 * dow 1..7 mon..sun
 * dow    8 mon..fri aka weekday
 * dow    9 sat..aun aka weekend
 *
 * T1 is the start time, T2 is the stop time.
 */
class WeekTimerLine
{
    private:
        int t1_dow;
        int t1_h;
        int t1_m;

        int t2_dow;
        int t2_h;
        int t2_m;

        static int timeSinceWeekStart(int d, int h, int m);

    public:
        WeekTimerLine();
        bool setLine(QString line); // 8:06:00-8:09:00

        bool isON(int dow, int h, int m);
};

#endif  // __WEEKTIMERLINE_H
