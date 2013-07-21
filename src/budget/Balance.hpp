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

#ifndef BALANCE_HPP
#define BALANCE_HPP

// UnderBudget include(s)
#include "accounting/Money.hpp"

// Forward declaration(s)
class QUndoCommand;

namespace ub {

/**
 * Generic monetary balance representation. A balance is the mathematical
 * sum of a set of defined contributing monetary values.
 */
class Balance : public QObject
{
	Q_OBJECT

public:
	/**
	 * Balance contributor.
	 */
	struct Contributor
	{
		/** Contributor name */
		QString name;
		/** Contribution amount */
		Money amount;
		/** Increase or decrease */
		bool increase;

		/** Default constructor */
		Contributor()
			: increase(true)
		{ }

		/** Parameterized constructor */
		Contributor(const QString& name, const Money& amount, bool increase)
			: name(name), amount(amount), increase(increase)
		{ }
	};

	// -- Constructors

	/**
	 * Creates a new balance from a single money value.
	 *
	 * @param[in] value single money value representing the balance
	 * @return new balance object instance
	 */
	static QSharedPointer<Balance> create(const Money& value = Money());

	/**
	 * Creates a new balance from the given set of contributors.
	 *
	 * @param[in] contributors contributing money values
	 * @return new balance object instance
	 */
	static QSharedPointer<Balance> create(const QList<Contributor>& contributors);

	// -- Modification methods (via command pattern)

	/**
	 * Creates an undoable command to add a new contributor
	 * to this balance.
	 *
	 * When the contributor is added or removed as a result of
	 * executing or undoing the created command, the `contributorAdded()`
	 * or `contributorRemoved()` signals are emitted.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* addContributor(QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to remove an existing contributor
	 * from this balance.
	 *
	 * When the contributor is removed or added as a result of
	 * exeucting or undoing the created command, the `contributorRemoved()`
	 * or `contributorAdded()` signals are emitted.
	 *
	 * @param[in] index index of the contributor to be removed
	 * @param[in] cmd   parent command to be used for grouping
	 */
	QUndoCommand* removeContributor(int index, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to update an existing contributor
	 * for this balance.
	 *
	 * When the contributor is updated as a result of executing or
	 * undoing the created command, the `contributorUpdated()` signal
	 * is emitted.
	 *
	 * @param[in] index       index of the contributor to be updated
	 * @param[in] contributor new contributor parameters
	 * @param[in] cmd         parent command to be used for grouping
	 */
	QUndoCommand* updateContributor(int index, const Contributor& contributor,
		QUndoCommand* cmd = 0);

	// -- Property access methods

	/**
	 * Returns the number of contributors to this balance.
	 *
	 * @return number of contributors to this balance
	 */
	int contributorCount() const;

	/**
	 * Returns the contributor to this balance at the given index.
	 *
	 * @param[in] index index at which to retrieve the balance contributor
	 * @return contributor at the specified index
	 */
	Contributor contributorAt(int index) const;

	/**
	 * Returns the total money value of this balance, which is a sum of
	 * the values of all contributing money values.
	 *
	 * @return total contributed amount
	 */
	Money value() const;

signals:
	/**
	 * Emitted when a new contributor is added to this balance as a result
	 * of the commands created by either `addContributor()` or
	 * `removeContributor()`.
	 *
	 * @param contributor new contributor that was added
	 * @param index       index of the new contributor
	 */
	void contributorAdded(const Contributor& contributor, int index);

	/**
	 * Emitted when a new contributor is removed from this balance as a result
	 * of the commands created by either `addContributor()` or
	 * `removeContributor()`.
	 *
	 * @param contributor contributor that was removed
	 * @param index       old index of the contributor
	 */
	void contributorRemoved(const Contributor& contributor, int index);

	/**
	 * Emitted when an existing contributor is updated as a result of the
	 * command created by `updateContributor()`.
	 *
	 * @param contributor new contributor parameters
	 * @param index       index of the updated contributor
	 */
	void contributorUpdated(const Contributor& contributor, int index);

private:
	/** Contributors */
	QList<Contributor> contributors;

	/**
	 * Constructs a new balance from a single money value.
	 *
	 * @param[in] value single money value representing the balance
	 */
	Balance(const Money& value);

	/**
	 * Constructs a new balance from the given set of contributors.
	 *
	 * @param[in] contributors contributing money values
	 */
	Balance(const QList<Contributor>& contributors);

	/**
	 * Adds the given contributor to this balance at the specified index.
	 * The `contributorAdded()` signal is emitted.
	 *
	 * @param[in] contributor new contributor
	 * @param[in] index       index at which to insert the new contributor
	 */
	void addContributor(const Contributor& contributor, int index);

	/**
	 * Removes the contributor located at the specified index. The
	 * `contributorRemoved()` signal is emitted.
	 *
	 * @param[in] index index of the contributor to be removed
	 */
	void deleteContributor(int index);

	/**
	 * Updates the contributor located at the specified index. The
	 * `contributorUpdated()` signal is emitted.
	 *
	 * @param[in] contributor new contributor parameters
	 * @param[in] index       index of the contributor to be updated
	 */
	void updateContributor(const Contributor& contributor, int index);

	// Allow undoable commands private access
	friend class AddContributorCommand;
	friend class RemoveContributorCommand;
	friend class UpdateContributorCommand;
};

}

// Make types known to Qt meta object system
Q_DECLARE_METATYPE(ub::Balance::Contributor);

#endif //BALANCE_HPP
