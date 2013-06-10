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

#ifndef ESTIMATE_HPP
#define ESTIMATE_HPP

// Qt include(s)
#include <QObject>

// UnderBudget include(s)
#include "accounting/Money.hpp"

// Forward declaration(s)
class QUndoCommand;

namespace ub {

/**
 * Representation of an estimated income or expense amount.
 */
class Estimate : public QObject
{
	Q_OBJECT

public:
	/**
	 * Estimate type enumeration.
	 */
	enum Type
	{
		/**
		 * Estimates that represent an increase in funds,
		 * such as earned wages or monetary received gifts.
		 */
		Income,
		/**
		 * Estimates that represent a decrease in funds,
		 * such as a purchase or gifts given.
		 */
		Expense,
		/**
		 * Estimates that represent a transfer of funds
		 * from one account to another.
		 */
		Transfer,
		/**
		 * Estimate tree root.
		 */
		Root,
	};

	/**
	 * Constructs a new root estimate.
	 */
	Estimate();

	/**
	 * Constructs a new estimate as a child of the given estimate.
	 *
	 * @param[in] parent      parent estimate
	 * @param[in] id          estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] dueDate     activity due date
	 * @param[in] finished    activity finished state
	 */
	Estimate(QSharedPointer<Estimate> parent, uint id,
		const QString& name, const QString& description, Type type,
		const Money& amount, const QDate& dueDate, bool finished);

	/**
	 * Constructs an estimate as a copy of an existing estimate.
	 *
	 * @param[in] orig original estimate
	 */
	Estimate(const Estimate& orig);

	/**
	 * Destroys the estimate. If this is the root estimate, the
	 * backing map of all estimates is destroyed.
	 */
	~Estimate();

	/**
	 * Returns the progress of this estimate as compared
	 * to the given actual figure.
	 *
	 * @param actual actual figure to be compared against
	 * @return progress of this estimate
	 */
	//Progress getProgress(const Actual& actual) const;

	/**
	 * Returns the impact of this estimate to the ending
	 * balances as determined with the given actual figure.
	 *
	 * @param actual actual figure to be used
	 * @return impact of this estimate to ending balances
	 */
	//Impact getImpact(const Actual& actual) const;

	// -- Modification methods (via command pattern)

	/**
	 * Creates a command to change the name of this estimate.
	 *
	 * @param[in] newName new name for the estimate
	 * @param[in] cmd     parent command to be used for grouping
	 */
	QUndoCommand* changeName(const QString& newName,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to change the description for
	 * this estimate.
	 *
	 * @param[in] newDescrip new description for the estimate
	 * @param[in] cmd        parent command to be used for grouping
	 */
	QUndoCommand* changeDescription(const QString& newDescrip,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to change the type of this
	 * estimate, and all its child estimates.
	 *
	 * @param[in] newType new type for the estimate tree
	 * @param[in] cmd     parent command to be used for grouping
	 */
	QUndoCommand* changeType(Type newType,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to change the amount of this
	 * estimate.
	 *
	 * @param[in] newAmt new estimated amount for this estimate
	 * @param[in] cmd    parent command to be used for grouping
	 */
	QUndoCommand* changeAmount(const Money& newAmt,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to set the due date for this
	 * estimate. A due date specifies a date by which
	 * activity should have occurred for an estimate.
	 * For example, a utility bill or credit card
	 * payment that is due by a particular date.
	 *
	 * @param[in] newDate new due date for this estimate
	 * @param[in] cmd     parent command to be used for grouping
	 */
	QUndoCommand* changeDueDate(const QDate& newDate,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to mark this estimate as
	 * finished. Finished estimates are estimates
	 * for which all expected activity has already
	 * occurred. This allows for their actual
	 * amounts to be used in calculating the
	 * expected ending balance.
	 *
	 * @param[in] newState whether the estimate is finished
	 * @param[in] cmd      parent command to be used for grouping
	 */
	QUndoCommand* changeFinishedState(bool newState,
		QUndoCommand* cmd = 0);

	/**
	 * Creates a command to create a new child estimate
	 * to this estimate.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* addChild(QUndoCommand* cmd = 0);

	/**
	 * Creates a command to delete this estimate.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* deleteEstimate(QUndoCommand* cmd = 0);

	// -- Tree traversal methods

	/**
	 * Returns the parent estimate for this estimate.
	 *
	 * @return parent estimate
	 */
	QSharedPointer<Estimate> parentEstimate() const;

signals:
	/**
	 * This signal is emitted whenever the name of this estimate
	 * changes.
	 *
	 * @param name current name of this estimate
	 */
	void nameChanged(const QString& name);

	/**
	 * This signal is emitted whenever the description for
	 * this estimate changes.
	 *
	 * @param description current description for this estimate
	 */
	void descriptionChanged(const QString& description);

	/**
	 * This signal is emitted whenever the type of this
	 * estimate changes.
	 *
	 * @param type current type of this estimate
	 */
	void typeChanged(Type type);

	/**
	 * This signal is emitted whenever the amount of this
	 * estimate changes.
	 *
	 * @param amount current amount of this estimate
	 */
	void amountChanged(const Money& amount);

	/**
	 * This signal is emitted whenever the due date of
	 * this estimate changes.
	 *
	 * @param date current due date for this estimate
	 */
	void dueDateChanged(const QDate& date);

	/**
	 * This signal is emitted whenever this estimate
	 * is marked as finished or unfinished.
	 *
	 * @param finished whether this estimate is finished
	 */
	void finishedChanged(bool finished);

	/**
	 * This signal is emitted whenever a child is
	 * added to this estimate.
	 *
	 * @param index index of the new child estimate
	 */
	void childAdded(int index);

	/**
	 * This signal is emitted when this estimate is
	 * deleted.
	 */
	void deleted();

private:
	/** Estimate pointer map */
	QSharedPointer<QHash<uint, QSharedPointer<Estimate> > > estimates;

	/** Parent estimate ID */
	uint parent;
	/** Child estimate IDs */
	QList<uint> children;

	/** Unique ID */
	const uint id;
	/** Name of this estimate */
	QString name;
	/** Description for this estimate */
	QString description;
	/** Type of this estimate */
	Type type;
	/** Amount of this estimate */
	Money amount;
	/** Due date for this estimate */
	QDate dueDate;
	/** Whether this estimate is finished */
	bool finished;

	/**
	 * Checks if this estimate is the root estimate.
	 *
	 * @return `true` if this estimate is the root
	 */
	bool isRoot() const;

	/**
	 * Checks if this estimate is a category of estimates.
	 *
	 * @return `true` if this estimate is a category of estimates
	 */
	bool isCategory() const;

	/**
	 * Sets the name of this estimate.
	 *
	 * @param[in] newName new name for this estimate
	 */
	void setName(const QString& newName);

	/**
	 * Sets the description for this estimate.
	 *
	 * @param[in] newDescrip new description for this estimate
	 */
	void setDescription(const QString& newDescrip);

	/**
	 * Sets the type of this estimate.
	 *
	 * @param[in] newType new type of this estimate
	 */
	void setType(Type newType);

	/**
	 * Sets the amount of this estimate.
	 *
	 * @param[in] newAmt new amount of this estimate
	 */
	void setAmount(const Money& newAmt);

	/**
	 * Sets the due date for this estimate.
	 *
	 * @param[in] newDate new due date for this estimate
	 */
	void setDueDate(const QDate& newDate);

	/**
	 * Marks this estimate as finished or unfinished.
	 *
	 * @param[in] newState whether this estimate is finished
	 */
	void setFinishedState(bool newState);

	/**
	 * Creates a new child estimate under this estimate.
	 * The parameters of the new estimate will be inherited
	 * from this estimate.
	 */
	QSharedPointer<Estimate> createChild();

	/**
	 * Creates a new child estimate under this estimate.
	 *
	 * @param[in] uid         estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] dueDate     due date
	 * @param[in] finished    whether activity is finished
	 */
	QSharedPointer<Estimate> createChild(long uid,
		const QString& name, const QString& description,
		Type type, const Money& amount,
		const QDate& dueDate, bool finished);

	/**
	 * Removes this estimate from its parent estimate.
	 * This estimate should be deleted after this
	 * method is called.
	 */
	void removeFromParent();

	// Allow undoable commands private access
	friend class ChangeEstimateAmountCommand;
	friend class ChangeEstimateDescriptionCommand;
	friend class ChangeEstimateDueDateCommand;
	friend class ChangeEstimateFinishedCommand;
	friend class ChangeEstimateNameCommand;
	friend class ChangeEstimateTypeCommand;
};

// Typedef this variable type since it's definition is quite wordy
typedef QSharedPointer<QHash<uint, QSharedPointer<Estimate> > > EstimatePointerMap;

}

// Make types known to Qt meta object system
Q_DECLARE_METATYPE(ub::Estimate::Type);

#endif // ESTIMATE_HPP
