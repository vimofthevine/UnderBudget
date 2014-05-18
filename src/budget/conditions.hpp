/*
 * Copyright 2013 Kyle Treubig
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

#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

// Qt include(s)
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

namespace ub {

// Forward declaration(s)
class Money;

/**
 * Checks if the given date meets the given condition's criteria.
 *
 * @param[in] date      date to be compared
 * @param[in] condition condition criteria to meet
 * @return `true` if the date matches the condition
 * @ingroup budget
 */
bool qualifies(const QDate& date, const AssignmentRule::Condition& condition);

/**
 * Checks if the given amount meets the given condition's criteria.
 *
 * @param[in] amount    amount to be compared
 * @param[in] condition condition criteria to meet
 * @return `true` if the amount matches the condition
 * @ingroup budget
 */
bool qualifies(const Money& amount, const AssignmentRule::Condition& condition);

/**
 * Checks if the given string meets the given condition's criteria.
 *
 * @param[in] string    string to be compared
 * @param[in] condition condition criteria to meet
 * @return `true` if the string matches the condition
 * @ingroup budget
 */
bool qualifies(const QString& string, const AssignmentRule::Condition& condition);

}

#endif //CONDITIONS_HPP
