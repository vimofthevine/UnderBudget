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

#ifndef ASSIGNMENTS_HPP
#define ASSIGNMENTS_HPP

// Qt include(s)
#include <QHash>
#include <QObject>

namespace ub {

/**
 * Records assignments of transactions to estimates, including the
 * assignment rule that governed the assignment.
 */
class Assignments : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a new assignments collection.
	 *
	 * @param[in] parent parent object
	 */
	Assignments(QObject* parent = 0);

	/**
	 * Clears all previous assignments.
	 */
	void clear();

	/**
	 * Records the assignment of a transaction to an estimate with
	 * a particular assignment rule.
	 *
	 * @param[in] trnId  unique ID of the transaction being assigned
	 * @param[in] estId  unique ID of the estimate to which the transaction
	 *                   is assigned
	 * @param[in] ruleId unique ID of the assignment rule for which the
	 *                   transaction qualified
	 */
	void record(uint trnId, uint estId, uint ruleId);

	/**
	 * Returns the number of assigned transactions statistic.
	 *
	 * @return number of assigned transactions
	 */
	int numberOfAssignments() const;

	/**
	 * Returns the unique ID of the estimate to which the given transaction
	 * has been assigned.
	 *
	 * @param[in] trnId unique ID of the transaction
	 * @return unique ID of the estimate to which the transaction was assigned,
	 *         or 0 if it was not assigned
	 */
	uint estimate(uint trnId) const;

	/**
	 * Returns the unique ID of the assignment rule for which the given transaction
	 * qualified.
	 *
	 * @param[in] trnId unique ID of the transaction
	 * @return unique ID of the assignment rule for which the transaction qualified,
	 *         or 0 if it was not assigned
	 */
	uint rule(uint trnId) const;

signals:
	/**
	 * Emitted whenever the assignments have changed. The change
	 * may be that the assignments have been cleared, or a new
	 * assignment has been recorded.
	 */
	void assignmentsChanged();

private:
	/** Map of transaction ID to assigned estimate ID */
	QHash<uint, uint> transactionToEstimate;
	/** Map of transaction ID to assigning rule */
	QHash<uint, uint> transactionToRule;
};

}

#endif //ASSIGNMENTS_HPP
