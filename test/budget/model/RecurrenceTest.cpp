/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

// Standard include(s)
#include <iostream>
#include <string>
#include <tuple>

// Google include(s)
#include <gtest/gtest.h>

// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include <budget/model/Recurrence.hpp>

/**
 * Prints the date to the given output stream.
 *
 * @param date Date
 * @param os   Output stream
 */
void PrintTo(const QDate & date, ::std::ostream * os) {
    *os << date.toString("yyyy-MM-dd").toStdString();
}

namespace ub {
namespace budget {

/** Recurrence test case */
struct Case {
    int day;
    int month;
    int periodicity;
    Recurrence::ScopeType scope;
    int week;
    QDate date;
    QDate expected;
    std::string what;
};

/**
 * Prints the test case to the given output stream.
 *
 * @param c  Test case
 * @param os Output stream
 */
void PrintTo(const Case & c, ::std::ostream * os) {
    *os << "day " << c.day << ", month " << c.month
        << ", periodicity " << c.periodicity
        << ", scope " << c.scope << ", week " << c.week
        << ", after " << c.date.toString("yyyy-MM-dd").toStdString();
}

/** Test fixture */
class RecurrenceTest : public ::testing::TestWithParam<Case> {};

/** Verifies that an invalid next occurrence is returned for a non-recurring recurrence */
TEST(RecurrenceTest, ShouldReturnInvalidNextOccurrenceWhenNotRecurring) {
    Recurrence r;
    EXPECT_FALSE(r.nextOccurrence(QDate(2017, 3, 14)).isValid());
}

/** Verifies that the next occurrence is correctly calculated */
TEST_P(RecurrenceTest, ShouldDetermineCorrectNextOccurence) {
    auto params = GetParam();
    Recurrence r;
    r.setDay(params.day);
    r.setMonth(params.month);
    r.setPeriodicity(params.periodicity);
    r.setScope(params.scope);
    r.setWeek(params.week);
    EXPECT_EQ(r.nextOccurrence(params.date), params.expected) << "Case failed: " << params.what;
}

INSTANTIATE_TEST_CASE_P(MonthInYearly, RecurrenceTest,
                        ::testing::Values(
                            Case{0, 5, 1, Recurrence::Yearly, 0, QDate(2017, 3, 1), QDate(2017, 5, 1), "Before start of month"},
                            Case{0, 5, 1, Recurrence::Yearly, 0, QDate(2017, 7, 1), QDate(2018, 5, 1), "After start of month"},
                            Case{0, 5, 1, Recurrence::Yearly, 0, QDate(2017, 5, 1), QDate(2018, 5, 1), "On start of month, one year"},
                            Case{0, 5, 2, Recurrence::Yearly, 0, QDate(2017, 5, 1), QDate(2019, 5, 1), "On start of month, two years"},
                            Case{0, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 1), QDate(2017, 5, 8), "Before start of week"},
                            Case{0, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 16), QDate(2018, 5, 14), "After start of week"},
                            Case{0, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 8), QDate(2018, 5, 14), "On start of week, one year"},
                            Case{5, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 10), QDate(2017, 5, 12), "Before day of week"},
                            Case{5, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 15), QDate(2018, 5, 11), "After day of week"},
                            Case{5, 5, 1, Recurrence::Yearly, 2, QDate(2017, 5, 12), QDate(2018, 5, 11), "On day of week"},
                            Case{1, 6, 1, Recurrence::Yearly, 2, QDate(2017, 6, 1), QDate(2017, 6, 12), "First of month is later day of week"},
                            Case{24, 5, 1, Recurrence::Yearly, 0, QDate(2017, 5, 2), QDate(2017, 5, 24), "Before day of month"},
                            Case{24, 5, 1, Recurrence::Yearly, 0, QDate(2017, 5, 26), QDate(2018, 5, 24), "After day of month"},
                            Case{24, 5, 1, Recurrence::Yearly, 0, QDate(2017, 5, 24), QDate(2018, 5, 24), "On day of month"}
                        ));

INSTANTIATE_TEST_CASE_P(WeekInYearly, RecurrenceTest,
                        ::testing::Values(
                            Case{0, 0, 1, Recurrence::Yearly, 13, QDate(2017, 1, 1), QDate(2017, 3, 27), "Before start of week"},
                            Case{0, 0, 1, Recurrence::Yearly, 13, QDate(2017, 5, 1), QDate(2018, 3, 26), "After start of week"},
                            Case{0, 0, 1, Recurrence::Yearly, 13, QDate(2017, 3, 27), QDate(2018, 3, 26), "On start of week, one year"},
                            Case{0, 0, 2, Recurrence::Yearly, 13, QDate(2017, 3, 27), QDate(2019, 4, 1), "On start of week, two years"},
                            Case{5, 0, 1, Recurrence::Yearly, 13, QDate(2017, 1, 1), QDate(2017, 3, 31), "Before day of week"},
                            Case{5, 0, 1, Recurrence::Yearly, 13, QDate(2017, 4, 5), QDate(2018, 3, 30), "After day of week"},
                            Case{5, 0, 1, Recurrence::Yearly, 13, QDate(2017, 3, 31), QDate(2018, 3, 30), "On day of week"}
                        ));

INSTANTIATE_TEST_CASE_P(Monthly, RecurrenceTest,
                        ::testing::Values(
                            Case{0, 0, 1, Recurrence::Monthly, 0, QDate(2017, 6, 10), QDate(2017, 7, 1), "After start of month"},
                            Case{0, 0, 1, Recurrence::Monthly, 0, QDate(2017, 6, 1), QDate(2017, 7, 1), "On start of month, one month"},
                            Case{0, 0, 2, Recurrence::Monthly, 0, QDate(2017, 6, 1), QDate(2017, 8, 1), "On start of month, two months"},
                            Case{0, 0, 1, Recurrence::Monthly, 0, QDate(2017, 12, 1), QDate(2018, 1, 1), "End of year"},
                            Case{0, 0, 1, Recurrence::Monthly, 0, QDate(2016, 2, 14), QDate(2016, 3, 1), "Leap Year"}
                        ));

INSTANTIATE_TEST_CASE_P(WeekInMonthly, RecurrenceTest,
                        ::testing::Values(
                            Case{0, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 10), QDate(2017, 6, 19), "Before start of week"},
                            Case{0, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 28), QDate(2017, 7, 17), "After start of week"},
                            Case{0, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 19), QDate(2017, 7, 17), "On start of week, one month"},
                            Case{0, 0, 2, Recurrence::Monthly, 3, QDate(2017, 6, 19), QDate(2017, 8, 21), "On start of week, one month"},
                            Case{1, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 10), QDate(2017, 6, 19), "Before day of week"},
                            Case{1, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 28), QDate(2017, 7, 17), "After day of week"},
                            Case{1, 0, 1, Recurrence::Monthly, 3, QDate(2017, 6, 19), QDate(2017, 7, 17), "On day of week"}
                        ));

INSTANTIATE_TEST_CASE_P(DayInMonthly, RecurrenceTest,
                        ::testing::Values(
                            Case{14, 0, 1, Recurrence::Monthly, 0, QDate(2017, 6, 10), QDate(2017, 6, 14), "Before day of month"},
                            Case{14, 0, 1, Recurrence::Monthly, 0, QDate(2017, 6, 18), QDate(2017, 7, 14), "After day of month"},
                            Case{14, 0, 1, Recurrence::Monthly, 0, QDate(2017, 6, 14), QDate(2017, 7, 14), "On day of month, one month"},
                            Case{14, 0, 2, Recurrence::Monthly, 0, QDate(2017, 6, 14), QDate(2017, 8, 14), "On day of month, one month"}
                        ));

INSTANTIATE_TEST_CASE_P(Weekly, RecurrenceTest,
                        ::testing::Values(
                            Case{0, 0, 1, Recurrence::Weekly, 0, QDate(2017, 9, 12), QDate(2017, 9, 18), "Before start of week"},
                            Case{0, 0, 1, Recurrence::Weekly, 0, QDate(2017, 9, 18), QDate(2017, 9, 25), "One start of week, one week"},
                            Case{0, 0, 2, Recurrence::Weekly, 0, QDate(2017, 9, 18), QDate(2017, 10, 2), "One start of week, two weeks"}
                        ));

INSTANTIATE_TEST_CASE_P(DayInWeekly, RecurrenceTest,
                        ::testing::Values(
                            Case{5, 0, 1, Recurrence::Weekly, 0, QDate(2017, 9, 12), QDate(2017, 9, 15), "Before day of week"},
                            Case{5, 0, 1, Recurrence::Weekly, 0, QDate(2017, 9, 16), QDate(2017, 9, 22), "After day of week"},
                            Case{5, 0, 1, Recurrence::Weekly, 0, QDate(2017, 9, 15), QDate(2017, 9, 22), "On day of week, one week"},
                            Case{5, 0, 2, Recurrence::Weekly, 0, QDate(2017, 9, 15), QDate(2017, 9, 29), "On day of week, one week"}
                        ));

} // budget namespace
} // ub namespace
