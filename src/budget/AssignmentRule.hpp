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

#ifndef ASSIGNMENTRULE_HPP
#define ASSIGNMENTRULE_HPP

// Qt include(s)
#include <QObject>

// Forward declaration(s)
class QUndoCommand;

namespace ub::budget
{
	/**
	 * A set of conditions to be matched against transactions
	 * and to assign the transaction to a specific estimate.
	 */
	class AssignmentRule : public QObject
	{
	public:

		/**
		 * Checks if the given transaction matches
		 * the criteria of this assignment rule.
		 *
		 * @param transaction transaction to be evaluated
		 * @return `true` if the transaction meets
		 *         the criteria of this rule, else `false`
		 */
		bool matches(const Transaction& transaction) const;

	private:
		/** Estimate UID */
		const QUuid estimateUid;
		/** Conditions */
		const QList<QSharedPointer<Condition> > myConditions;

		/**
		 * Adds the given condition to this assignment rule.
		 *
		 * @param[in] condition new condition to be added
		 */
		void add(QSharedPointer<Condition> > condition);
	};
}

#endif // ASSIGNMENTRULE_HPP
