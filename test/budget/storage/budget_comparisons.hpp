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

#ifndef BUDGET_COMPARISONS_HPP
#define BUDGET_COMPARISONS_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"
#include "budget/Estimate.hpp"

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Money& money)
{
	QString str = QString("%1 (%2, %3)").arg(money.toString())
		.arg(QVariant(money.amount()).toString()).arg(money.currency().code());
	return toString(str);
}

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Estimate::Type& type)
{
	return toString(toString(type));
}

//------------------------------------------------------------------------------
template<>
char* toString(const ub::AssignmentRule::Field& field)
{
	return toString(toString(field));
}

//------------------------------------------------------------------------------
template<>
char* toString(const ub::AssignmentRule::Operator& oper)
{
	return toString(toString(oper));
}

}

/**
 * Compares the given estimate against all expected parameters.
 *
 * @param[in] actual   actual estimate to be compared
 * @param[in] id       expected estimate ID
 * @param[in] name     expected name
 * @param[in] descrip  expected description
 * @param[in] type     expected estimate type
 * @param[in] amount   expected estimated amount
 * @param[in] due      expected due date
 * @param[in] finished expected finished state
 * @param[in] children expected number of children
 */
#define COMPARE_ESTIMATE(actual, id, name, descrip, type, amount, due, finished, children) \
	qDebug() << "comparing estimate id =" << id; \
	QCOMPARE(actual->estimateId(), id); \
	QCOMPARE(actual->estimateName(), name); \
	QCOMPARE(actual->estimateDescription(), descrip); \
	QCOMPARE(actual->estimateType(), type); \
	QCOMPARE(actual->estimatedAmount(), amount); \
	QCOMPARE(actual->activityDueDateOffset(), due); \
	QCOMPARE(actual->isActivityFinished(), finished); \
	QCOMPARE(actual->childCount(), children);

/**
 * Compares the given assignment rule against all expected parameters.
 *
 * @param[in] actual     actual assignment rule to be compared
 * @param[in] eid        expected associated estimate ID
 * @param[in] conditions expected number of conditions
 */
#define COMPARE_RULE(actual, eid, conditions) \
	qDebug() << "comparing rule id =" << actual->ruleId(); \
	QCOMPARE(actual->estimateId(), eid); \
	QCOMPARE(actual->conditionCount(), conditions);

/**
 * Compares the given assignment rule against all expected parameters.
 *
 * @param[in] actual     actual assignment rule to be compared
 * @param[in] rid        expected rule ID
 * @param[in] eid        expected associated estimate ID
 * @param[in] conditions expected number of conditions
 */
#define COMPARE_RULE_ID(actual, rid, eid, conditions) \
	qDebug() << "comparing rule id =" << rid; \
	QCOMPARE(actual->ruleId(), rid); \
	QCOMPARE(actual->estimateId(), eid); \
	QCOMPARE(actual->conditionCount(), conditions);

/**
 * Compares the given rule condition against all expected parameters.
 *
 * @param[in] actual    actual rule condition to be compared
 * @param[in] fld       expected transaction field
 * @param[in] oper      expected comparison operator
 * @param[in] csense    expected case-sensitivity
 * @param[in] val       expected comparison value
 */
#define COMPARE_CONDITION(actual, fld, oper, csense, val) \
	QCOMPARE(actual.field, fld); \
	QCOMPARE(actual.op, oper); \
	QCOMPARE(actual.sensitive, csense); \
	QCOMPARE(actual.value, val);

#endif //BUDGET_COMPARISONS_HPP
