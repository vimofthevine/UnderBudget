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

#ifndef TRANSACTIONASSIGNERTEST_HPP
#define TRANSACTIONASSIGNERTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the TransactionAssigner class.
 */
class TransactionAssignerTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the acruing of actual values as a result of
	 * transaction assignment.
	 */
	void actualsFromAssignment();

	/**
	 * Test data for testing actual values from assignment.
	 */
	void actualsFromAssignment_data();

	/**
	 * Tests the estimate associations as a result of
	 * transaction assignment.
	 */
	void estimateAssociation();

	/**
	 * Test data for testing estimate association.
	 */
	void estimateAssociation_data();

	/**
	 * Tests the rule associations as a result of
	 * transaction assignment.
	 */
	void ruleAssociation();

	/**
	 * Test data for testing rule association.
	 */
	void ruleAssociation_data();
};

}

#endif //TRANSACTIONASSIGNERTEST_HPP
