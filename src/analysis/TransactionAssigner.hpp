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

#ifndef TRANSACTIONASSIGNER_HPP
#define TRANSACTIONASSIGNER_HPP

// Qt include(s)
#include <QList>
#include <QObject>
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

namespace ub {

// Forward declaration(s)
class Actuals;
class Assignments;
class AssignmentRules;
class ImportedTransaction;

/**
 * Assigns transactions to estimates according to the ordered
 * list of assignment rules.
 */
class TransactionAssigner : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a transaction assigner.
	 *
	 * @param[in] rules       assignment rules
	 * @param[in] assignments transaction assignments
	 * @param[in] actuals     estimate actuals
	 * @param[in] parent      parent object
	 */
	TransactionAssigner(QSharedPointer<AssignmentRules> rules,
		Assignments* assignments, Actuals* actuals, QObject* parent = 0);

public slots:
	/**
	 * Initiates an assignment of the given transactions to
	 * estimates with the current set of assignment rules.
	 *
	 * @param[in] transactions transactions to be assigned
	 */
	void assign(const QList<ImportedTransaction>& transactions);

signals:
	/**
	 * Emitted when an assignment operation commences.
	 */
	void started();

	/**
	 * If emitted, indicates the current progress of the
	 * assignment as a percentage (out of 100).
	 *
	 * @param percent assignment percent complete
	 */
	void progress(int percent);

	/**
	 * Emitted when the assignment operation is completed.
	 */
	void finished();

private:
	/** Assignment rules */
	QSharedPointer<AssignmentRules> rules;
	/** Transaction/estimate assignments */
	Assignments* assignments;
	/** Estimate actuals */
	Actuals* actuals;
	/** Whether the assigner is currently assigning */
	bool isAssigning;

	/**
	 * Assigns the given transaction, iterating over the list of
	 * assignment rules for a match.
	 *
	 * @param[in] transaction transaction to be assigned
	 */
	void assign(const ImportedTransaction& transaction);

	/**
	 * Checks if the given transaction matches the given assignment
	 * rule.
	 *
	 * @param[in] transaction transaction to be compared
	 * @param[in] rule        assignment rule to be checked
	 */
	bool matches(const ImportedTransaction& transaction,
		const AssignmentRule* rule);

	/**
	 * Checks if the given transaction qualifies for the given
	 * assignment condition.
	 *
	 * @param[in] transaction transaction to be compared
	 * @param[in] condition   assignment condition to be checked
	 */
	bool matches(const ImportedTransaction& transaction,
		const AssignmentRule::Condition& condition);
};

}

#endif //TRANSACTIONASSIGNER_HPP
