/**
 * @file TestWeekTimer.cpp
 * @author Johan Simonsson
 * @brief Tests for the WeekTimer (and WeekTimerLine)
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

#include <QtCore>
#include <QtTest>

#include "WeekTimer.h"
#include "WeekTimerLine.h"

class TestWeekTimer : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_setLine();
        void test_setLine_data();

        void test_isON_01(); //Will test normal days
        void test_isON_01_data();

        void test_isON_02(); //Will test wildcard dow=0 all days
        void test_isON_03(); //Will test wildcard dow=8 weekdays
        void test_isON_04(); //Will test wildcard dow=9 weekends

        void test_timeSinceWeekStart();
        void test_timeSinceWeekStart_data();

        void test_WeekTimer();
};

void TestWeekTimer::test_setLine_data()
{
    QTest::addColumn<QString>("timerLine");
    QTest::addColumn<bool>("valid");

    QTest::addColumn<int>("tt1_dow");
    QTest::addColumn<int>("tt1_h");
    QTest::addColumn<int>("tt1_m");
    QTest::addColumn<int>("tt2_dow");
    QTest::addColumn<int>("tt2_h");
    QTest::addColumn<int>("tt2_m");

    //QTest::newRow("normal data 01") << "temperature=54.34" << (int)DATAPOINT_SENSOR << false << "54.34" << "" << "" << "";
    QTest::newRow("test") << "1 06 00 - 1 09 00" << false << -1 << 0 << 0 << 0 << 0 << 0;
    QTest::newRow("test") << "1:06:00-1:09"      << false << -1 << 0 << 0 << 0 << 0 << 0;

    QTest::newRow("test") << "1:06:00-1:09:00" << true << 1 << 6 << 0 << 1 << 9 << 0;
    QTest::newRow("test") << "1:6:0-1:9:0"     << true << 1 << 6 << 0 << 1 << 9 << 0;

    QTest::newRow("test") << "7:12:32-7:23:34" << true << 7 << 12 << 32 << 7 << 23 << 34;
}

void TestWeekTimer::test_setLine()
{
    QFETCH(QString,timerLine);
    QFETCH(bool ,valid);

    QFETCH(int, tt1_dow);
    QFETCH(int, tt1_h);
    QFETCH(int, tt1_m);
    QFETCH(int, tt2_dow);
    QFETCH(int, tt2_h);
    QFETCH(int, tt2_m);

    WeekTimerLine wtl;
    QCOMPARE(wtl.setLine(timerLine), valid);

    QCOMPARE(wtl.t1_dow, tt1_dow);
    QCOMPARE(wtl.t1_h, tt1_h);
    QCOMPARE(wtl.t1_m, tt1_m);
    QCOMPARE(wtl.t2_dow, tt2_dow);
    QCOMPARE(wtl.t2_h, tt2_h);
    QCOMPARE(wtl.t2_m, tt2_m);
}

void TestWeekTimer::test_isON_01_data()
{
    //T1 is turn ON time, and T2 is turn OFF time
    QTest::addColumn<int>("tt1_dow"); //Test Time 1 Day Of Week
    QTest::addColumn<int>("tt1_h");   //Test Time 1 Hour
    QTest::addColumn<int>("tt1_m");
    QTest::addColumn<int>("tt2_dow");
    QTest::addColumn<int>("tt2_h");
    QTest::addColumn<int>("tt2_m");

    QTest::newRow("test mon 6 - mon 9")    << 1 << 6 << 0 << 1 << 9 << 0;
    QTest::newRow("test mon 6 - tue 6")    << 1 << 6 << 0 << 2 << 6 << 0;
    QTest::newRow("test wed 4 - wed 4:30") << 3 << 4 << 0 << 3 << 4 << 30;


    //Wrappover week!
    QTest::newRow("test sun 22 - mon 3") << 7 << 22 << 0 << 1 << 3 << 0;

    //More or less inverted week! (just off 6:20-6:30)
    QTest::newRow("test tue 6:30 - tue 6:20")  << 2 << 6 << 30 << 2 << 6 << 20;
}

void TestWeekTimer::test_isON_01()
{
    QFETCH(int, tt1_dow);
    QFETCH(int, tt1_h);
    QFETCH(int, tt1_m);
    QFETCH(int, tt2_dow);
    QFETCH(int, tt2_h);
    QFETCH(int, tt2_m);

    QString timerLine = QString("%1:%2:%3-%4:%5:%6")
        .arg(tt1_dow)
        .arg(tt1_h)
        .arg(tt1_m)
        .arg(tt2_dow)
        .arg(tt2_h)
        .arg(tt2_m);

    qDebug() << timerLine;


    WeekTimerLine wtl;
    QVERIFY(wtl.setLine(timerLine));

    bool foundON = false;
    bool foundOFF = false;


    bool on1=wtl.isON(1, 0, 0); //last status
    bool on2=on1;               //current status

    for( int day=1 ; day<=7 ; day++ )
    {
        for( int hour=0 ; hour<=23 ; hour++ )
        {
            for( int min=0 ; min<=59 ; min+=1 )
            {
                on2 = wtl.isON(day, hour, min);
                if(on2 != on1)
                {
                    on1=on2;
                    if(on1)
                    {
                        qDebug() << "New value ON :" << day << hour << min;
                        foundON = true;
                        QCOMPARE(day, tt1_dow);
                        QCOMPARE(hour, tt1_h);
                        QCOMPARE(min,  tt1_m);
                    }
                    else
                    {
                        qDebug() << "New value OFF:" << day << hour << min;
                        foundOFF = true;
                        QCOMPARE(day, tt2_dow);
                        QCOMPARE(hour, tt2_h);
                        QCOMPARE(min,  tt2_m);
                    }
                }
            }
        }
    }

    QVERIFY(foundON);
    QVERIFY(foundOFF);
}


void TestWeekTimer::test_isON_02()
{
    WeekTimerLine wtl;
    //All days between 04:00 and 06:00
    QVERIFY( wtl.setLine(QString("0:4:0-0:6:0")) );

    for( int day=1 ; day<=7 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), true);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,22, 0), false);
    }

    //All days between 06:00 and 04:00
    //more or less the all the time...!
    QVERIFY( wtl.setLine(QString("0:6:0-0:4:0")) );

    for( int day=1 ; day<=7 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), true);
        QCOMPARE( wtl.isON(day, 3, 0), true);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), true);
        QCOMPARE( wtl.isON(day,22, 0), true);
    }
}

void TestWeekTimer::test_isON_03()
{
    WeekTimerLine wtl;
    //All weekdays between 04:00 and 06:00
    QVERIFY( wtl.setLine(QString("8:4:0-8:6:0")) );

    for( int day=1 ; day<=5 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), true);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,22, 0), false);
    }
    for( int day=6 ; day<=7 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,22, 0), false);
    }


    //All weekdays between 22:00 and 02:00 the next day!
    //so on friday that will be off on sat.
    QVERIFY( wtl.setLine(QString("8:22:0-8:2:0")) );


    //mon is day after weekend, so we start in OFF
    QCOMPARE( wtl.isON(1, 1, 0), false);
    QCOMPARE( wtl.isON(1, 3, 0), false);
    QCOMPARE( wtl.isON(1, 5, 0), false);
    QCOMPARE( wtl.isON(1, 7, 0), false);
    QCOMPARE( wtl.isON(1,23, 0), true);

    for( int day=2 ; day<=5 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 1, 0), true);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,23, 0), true);
    }

    //Sat (day after weekday)
    QCOMPARE( wtl.isON(6, 1, 0), true);
    QCOMPARE( wtl.isON(6, 3, 0), false);
    QCOMPARE( wtl.isON(6, 5, 0), false);
    QCOMPARE( wtl.isON(6, 7, 0), false);
    QCOMPARE( wtl.isON(6,23, 0), false);
}



void TestWeekTimer::test_isON_04()
{
    WeekTimerLine wtl;
    //All weekends between 04:00 and 06:00
    QVERIFY( wtl.setLine(QString("9:4:0-9:6:0")) );

    for( int day=1 ; day<=5 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,22, 0), false);
    }
    for( int day=6 ; day<=7 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 0, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), true);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,22, 0), false);
    }


    //All weekends between 22:00 and 02:00 the next day!
    //so on sun that will be off on mon.
    QVERIFY( wtl.setLine(QString("9:22:0-9:2:0")) );


    //mon is day after weekend, so we start in ON
    QCOMPARE( wtl.isON(1, 1, 0), true);
    QCOMPARE( wtl.isON(1, 3, 0), false);
    QCOMPARE( wtl.isON(1, 5, 0), false);
    QCOMPARE( wtl.isON(1, 7, 0), false);
    QCOMPARE( wtl.isON(1,23, 0), false);

    for( int day=2 ; day<=5 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 1, 0), false);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,23, 0), false);
    }

    for( int day=6 ; day<=7 ; day++ )
    {
        QCOMPARE( wtl.isON(day, 1, 0), true);
        QCOMPARE( wtl.isON(day, 3, 0), false);
        QCOMPARE( wtl.isON(day, 5, 0), false);
        QCOMPARE( wtl.isON(day, 7, 0), false);
        QCOMPARE( wtl.isON(day,23, 0), true);
    }
}

void TestWeekTimer::test_timeSinceWeekStart_data()
{
    QTest::addColumn<int>("d");
    QTest::addColumn<int>("h");
    QTest::addColumn<int>("m");

    QTest::addColumn<int>("time");

    QTest::newRow("test") << 1 << 4 << 50 << (1*24*60)+(4*60)+50;

    QTest::newRow("test") << 1 << 0 <<  0 << 1*24*60;
    QTest::newRow("test") << 1 << 6 <<  0 << (1*24*60)+(6*60);
    QTest::newRow("test") << 1 << 9 <<  0 << (1*24*60)+(9*60);
    QTest::newRow("test") << 1 << 9 << 30 << (1*24*60)+(9*60)+30;

    QTest::newRow("test") << 5 << 9 << 30 << (5*24*60)+(9*60)+30;
}

void TestWeekTimer::test_timeSinceWeekStart()
{

    QFETCH(int, d);
    QFETCH(int, h);
    QFETCH(int, m);
    QFETCH(int, time);


    WeekTimerLine wtl;
    int timeCheck = wtl.timeSinceWeekStart(d,h,m);

    //qDebug() << d << h << m << timeCheck << time;
    QCOMPARE(timeCheck, time);
}


void TestWeekTimer::test_WeekTimer()
{
    WeekTimer wt("Rum1");
    QCOMPARE(QString("Rum1"), wt.name);

    QCOMPARE(wt.isName(QString("Rum1")), true);
    QCOMPARE(wt.isName(QString("Rum2")), false);

    wt.addNewTimers(QString("1:02:00-1:03:00;1:04:00-1:05:00;1:06:00-1:07:00"));
    QCOMPARE(3, wt.timers.size());

    QCOMPARE(wt.isON(1,1,30), false);
    QCOMPARE(wt.isON(1,2,30), true);
    QCOMPARE(wt.isON(1,3,30), false);
    QCOMPARE(wt.isON(1,4,30), true);
    QCOMPARE(wt.isON(1,5,30), false);
    QCOMPARE(wt.isON(1,6,30), true);
    QCOMPARE(wt.isON(1,7,30), false);


    QCOMPARE(-1, wt.id);
    wt.setID(2);
    QCOMPARE(2, wt.id);

    QCOMPARE(wt.getID(), 2);
}


QTEST_MAIN(TestWeekTimer)
#include "TestWeekTimer.moc"
