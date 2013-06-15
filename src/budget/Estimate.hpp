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
	 * Creates a new root estimate.
	 */
	static QSharedPointer<Estimate> createRoot();

	/**
	 * Creates a new estimate as a child of the given estimate.
	 *
	 * If an index is specified (and is within a valid range), the estimate
	 * will be inserted at that index into the parent estimate's children.
	 * If the index is -1 or is invalid (e.g., specifying 5 when the parent
	 * only has 2 children), the new child will be inserted at the end of
	 * the parent's child list.
	 *
	 * @param[in] parent      parent estimate
	 * @param[in] id          estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] dueDate     activity due date
	 * @param[in] finished    activity finished state
	 * @param[in] index       child index
	 */
	static QSharedPointer<Estimate> create(QSharedPointer<Estimate> parent,
		uint id, const QString& name, const QString& description, Type type,
		const Money& amount, const QDate& dueDate, bool finished, int index = -1);

	/**
	 * Constructs an estimate as a copy of an existing estimate.
	 *
	 * @param[in] orig original estimate
	 */
	Estimate(const Estimate& orig);

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

	/**
	 * Creates a command to move this estimate.
	 *
	 * @param[in] newParent new parent to which this estimate will be moved
	 * @param[in] newIndex  new index under the parent
	 * @param[in] cmd       parent command to be used for grouping
	 */
	QUndoCommand* moveTo(QSharedPointer<Estimate> newParent, int newIndex,
		QUndoCommand* cmd = 0);

	// -- Property access methods

	/**
	 * Returns the pointer to this estimate.
	 *
	 * @return pointer to this estimate
	 */
	QSharedPointer<Estimate> operator&() const;

	/**
	 * Returns the unique ID of this estimate.
	 *
	 * @return unique ID of this estimate
	 */
	uint estimateId() const;

	/**
	 * Returns the name of this estimate.
	 *
	 * @return name of this estimate
	 */
	QString estimateName() const;

	/**
	 * Returns the description of this estimate.
	 *
	 * @return description of this estimate
	 */
	QString estimateDescription() const;

	/**
	 * Returns the type of this estimate.
	 *
	 * @return type of this estimate
	 */
	Type estimateType() const;

	/**
	 * Returns the amount of this estimate.
	 *
	 * @return amount of this estimate
	 */
	Money estimatedAmount() const;

	/**
	 * Returns the due date for this estimate.
	 *
	 * @returns due date for this estimate
	 */
	QDate activityDueDate() const;

	/**
	 * Returns whether activity has finished for this estimate.
	 *
	 * @return `true` if estimate activity has finished
	 */
	bool isActivityFinished() const;

	// -- Tree traversal methods

	/**
	 * Returns the parent estimate for this estimate.
	 *
	 * @return parent estimate
	 */
	QSharedPointer<Estimate> parentEstimate() const;

	/**
	 * Returns the number of child estimates under this estimate.
	 *
	 * @return number of child estimates
	 */
	int childCount() const;

	/**
	 * Returns the child estimate at the given index.
	 *
	 * @return child estimate at the given index
	 */
	QSharedPointer<Estimate> childAt(int index) const;

	/**
	 * Returns the child index of the given child estimate,
	 * if it is a parent of this estimate.
	 *
	 * @return index of the given child index, or -1 if the estimate
	 *         is not a child of this estimate
	 */
	int indexOf(QSharedPointer<Estimate> child) const;

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
	 * @param child new child estimate that was added
	 * @param index index of the new child estimate
	 */
	void childAdded(QSharedPointer<Estimate> child, int index);

	/**
	 * This signal is emitted whenever a child is removed
	 * from this estimate.
	 *
	 * Note that the child estimate given in this signal does not
	 * necessarily have a valid parent reference.
	 *
	 * @param child child estimate that was removed
	 * @param index old index of the child
	 */
	void childRemoved(QSharedPointer<Estimate> child, int index);

	/**
	 * This signal is emitted whenever a child is moved within
	 * this estimate's list of children.
	 *
	 * @param child    child estimate that was moved
	 * @param oldIndex old index of the child estimate
	 * @param newIndex new index of the child estimate
	 */
	void childMoved(QSharedPointer<Estimate> child, int oldIndex, int newIndex);

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
	 * Constructs a new root estimate.
	 */
	Estimate();

	/**
	 * Constructs a new estimate as a child of the given estimate.
	 *
	 * If an index is specified (and is within a valid range), the estimate
	 * will be inserted at that index into the parent estimate's children.
	 * If the index is -1 or is invalid (e.g., specifying 5 when the parent
	 * only has 2 children), the index will be set to the index at which the
	 * new child estimate was inserted.
	 *
	 * @param[in] parent      parent estimate
	 * @param[in] id          estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] dueDate     activity due date
	 * @param[in] finished    activity finished state
	 * @param[in] index       child index
	 */
	Estimate(QSharedPointer<Estimate> parent, uint id, const QString& name,
		const QString& description, Type type, const Money& amount,
		const QDate& dueDate, bool finished, int index);

	/**
	 * Returns the pointer to this estimate.
	 *
	 * @return pointer to this estimate
	 */
	QSharedPointer<Estimate> pointer() const;

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
	 * Retrieves the total estimated amount for all estimates that
	 * are descendants of this estimate.
	 *
	 * @return total estimated amount for all descendants
	 */
	Money totalEstimatedAmount() const;

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
	 *
	 * @return ID of the newly created child estimate
	 */
	uint createChild();

	/**
	 * Creates a new child estimate under this estimate with
	 * the given parameters.
	 *
	 * @param[in] id          estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] dueDate     activity due date
	 * @param[in] finished    activity finished state
	 * @param[in] index       index at which to insert the new estimate into
	 *                        the list of child estimates
	 */
	void createChild(uint id, const QString& name,
		const QString& description, Type type, const Money& amount,
		const QDate& dueDate, bool finished, int index = -1);

	/**
	 * Removes this estimate from the internal estimate pointer
	 * map and from its parent estimate.
	 */
	void deleteSelf();

	/**
	 * Moves this estimate to the specified parent at the specified index.
	 *
	 * @param[in] newParentId new parent under which to move this estimate
	 * @param[in] newIndex    new index at which to move this estimate
	 */
	void moveTo(uint newParentId, int newIndex);

	// Parent-child relationship modification methods

	/**
	 * Adds the given child to this estimate at the specified index.
	 *
	 * Emits the childAdded() signal.
	 *
	 * @param[in] child child estimate to be added
	 * @param[in] index child index
	 */
	void addChild(QSharedPointer<Estimate> child, int index);

	/**
	 * Removes the given child from this estimate.
	 *
	 * Emits the childRemoved() signal.
	 *
	 * @param[in] child child estimate to be removed
	 */
	void removeChild(QSharedPointer<Estimate> child);

	/**
	 * Moves the given child to the given index.
	 *
	 * Emits the childMoved() signal.
	 *
	 * @param[in] child child estimate to be moved
	 * @param[in] index new index of the child estimate
	 */
	void moveChild(QSharedPointer<Estimate> child, int index);

	// Allow undoable commands private access
	friend class AddChildEstimateCommand;
	friend class ChangeEstimateAmountCommand;
	friend class ChangeEstimateDescriptionCommand;
	friend class ChangeEstimateDueDateCommand;
	friend class ChangeEstimateFinishedCommand;
	friend class ChangeEstimateNameCommand;
	friend class ChangeEstimateTypeCommand;
	friend class DeleteEstimateCommand;
	friend class MoveEstimateCommand;
};

// Typedef this variable type since it's definition is quite wordy
typedef QSharedPointer<QHash<uint, QSharedPointer<Estimate> > > EstimatePointerMap;

}

// Make types known to Qt meta object system
Q_DECLARE_METATYPE(ub::Estimate::Type);

#endif // ESTIMATE_HPP
