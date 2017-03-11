/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

// Qt include(s)
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
    int day() const;

    /**
     * Returns the month within the scope in which the recurrency occurs.
     *
     * A value of 0 indicates that the month is not relevant, or the last month of the
     * scope is to be used if no other constraints are given.
     *
     * @return Month within the recurrency scope
     */
    int month() const;

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
    int periodicity() const;

    /**
     * Updates the day within the scope.
     *
     * @param day New day
     */
    void setDay(int day);

    /**
     * Updates the month within the scope.
     *
     * @param month New month
     */
    void setMonth(int month);

    /**
     * Updates the recurrence periodicity.
     *
     * @param periodicity New periodicity
     */
    void setPeriodicity(int periodicity);

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
    void setWeek(int week);

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
    int week() const;

private:
    /** Day within the scope */
    int day_;
    /** Month within the scope */
    int month_;
    /** Recurrence periodicity */
    int periodicity_;
    /** Scope type */
    ScopeType scope_;
    /** Week within the scope */
    int week_;
};

} // budget namespace
} // ub namespace
