/**
 * @file TestSQLiteWrapper.cpp
 * @author Johan Simonsson
 * @brief Tests for the SQLiteWrapper (and SQLiteWrapperLine)
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

#include "SQLiteWrapper.h"

class TestSQLiteWrapper : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void initTestCase();
        void cleanupTestCase();

        //Tests for SQLiteWrapper
        void test_SQLiteWrapper();
};

extern unsigned int MY_FEJK_TIME;

void TestSQLiteWrapper::initTestCase()
{
    //qDebug("called before everything else");
    MY_FEJK_TIME = 1;
}

void TestSQLiteWrapper::cleanupTestCase()
{
    //qDebug("called after...");
    //QFile file("weektimer.db");
    //file.remove();
}

void TestSQLiteWrapper::test_SQLiteWrapper()
{
    SQLiteWrapper db;

    QString name("r1");
    QString time("0:10:00-0:11:00");

    //Insert
    QVERIFY(db.updateWeekTimer(name, time));
    QCOMPARE(db.getWeekTimer(name), time);
    //Update
    time.append(";1:20:00-1:21:00");
    QVERIFY(db.updateWeekTimer(name, time));
    QCOMPARE(db.getWeekTimer(name), time);


    /// @todo Test more on the Force stuff

    //Start at time 100.
    MY_FEJK_TIME = 100;

    //Insert
    QVERIFY(db.updateForce(name, "ON",    0));
    QVERIFY(db.updateForce(name, "OFF", 100));
    QVERIFY(db.updateForce(name, "AUTO",100));
    //QCOMPARE(db.getForce(name), time);

}


QTEST_MAIN(TestSQLiteWrapper)
#include "TestSQLiteWrapper.moc"
