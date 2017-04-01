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

#pragma once

// Standard include(s)
#include <cstdint>

// Qt include(s)
#include <QDate>
#include <QMetaType>
#include <QString>

namespace ub {
namespace budget {

/**
 * The parameters defining the recurrence of a budgeted event (e.g., an expense).
 *
 * @ingroup budget
 */
class Recurrence {
public:
    /** Scope of the recurrence */
    enum ScopeType {
        /** Once-in-a-year scope */
        Yearly,
        /** Once-in-a-month scope */
        Monthly,
        /** Once-in-a-week scope */
        Weekly
    };

    /**
     * Initializes a recurrence with default values.
     */
    Recurrence();

    /**
     * Initializes a recurrence with the given values.
     */
    Recurrence(int day, int month, int periodicity, ScopeType scope, int week);

    // Copy constructor
    Recurrence(const Recurrence & orig) = default;

    // Assignment operator
    Recurrence & operator=(const Recurrence & orig) = default;

    /**
     * Returns the day within the scope on which the recurrence occurs.
     *
     * A value of 0 indicates that the day is not relevant, or the last day of the
     * scope is to be used if no other constraints are given.
     *
     * @return Day within the recurrency scope
     */
    int32_t day() const;

    /**
     * Returns the month within the scope in which the recurrency occurs.
     *
     * A value of 0 indicates that the month is not relevant, or the last month of the
     * scope is to be used if no other constraints are given.
     *
     * @return Month within the recurrency scope
     */
    int32_t month() const;

    /**
     * Determines the first date after the given date on which this occurrence will occur.
     *
     * @param date Date after which to determine the next occurrence
     * @return Date of next occurrence, or an invalid date if no occurence would occur after
     *         the given date
     */
    QDate nextOccurrence(const QDate & date) const;

    /**
     * Returns the periodicity at which the recurrence is applied.
     *
     * That is, a value of 2 means that the recurrence is every other <scope>. A value of 1
     * means every <scope>.
     *
     * A value of 0 indicates that the recurrence is not active.
     *
     * @return Periodicity of the recurrence
     */
    int32_t periodicity() const;

    /**
     * Updates the day within the scope.
     *
     * @param day New day
     */
    void setDay(int32_t day);

    /**
     * Updates the month within the scope.
     *
     * @param month New month
     */
    void setMonth(int32_t month);

    /**
     * Updates the recurrence periodicity.
     *
     * @param periodicity New periodicity
     */
    void setPeriodicity(int32_t periodicity);

    /**
     * Updates the scope type of the recurrence.
     *
     * @param scope New scope
     */
    void setScope(ScopeType scope);

    /**
     * Updates the week within the scope.
     *
     * @param week New week
     */
    void setWeek(int32_t week);

    /**
     * Returns the scope type of the recurrence.
     *
     * @return Scope type of the recurrence
     */
    ScopeType scope() const;

    /**
     * Returns a string representation of this recurrence.
     *
     * @return String representation of this recurrence
     */
    QString toString() const;

    /**
     * Returns the week within the scope in which the recurrency occurs.
     *
     * A value of 0 indicates that the week is not relevant, or the last week of the
     * scope is to be used if no other constraints are given.
     *
     * @return Week within the recurrency scope
     */
    int32_t week() const;

private:
    /** Day within the scope */
    int32_t day_;
    /** Month within the scope */
    int32_t month_;
    /** Recurrence periodicity */
    int32_t periodicity_;
    /** Scope type */
    ScopeType scope_;
    /** Week within the scope */
    int32_t week_;

    /**
     * Returns the date in the given month that satisfies the day
     * of the month requirements of this recurrence.
     *
     * @param date First date of the month in which to start
     * @return
     */
    QDate goToDayOfMonth(const QDate & date) const;

    /**
     * Returns the next date after the given date that satisfies the day
     * of the week requirements of this recurrence.
     *
     * @param date Date from which to start
     * @return Next date on the correct day of the week
     */
    QDate goToDayOfWeek(const QDate & date) const;
};

} // budget namespace
} // ub namespace

// Make Recurrence::ScopeType known to QMetaType
Q_DECLARE_METATYPE(ub::budget::Recurrence::ScopeType)
