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
 *
 * Estimates are one of three types of estimate.
 * <ul>
 *   <li>Expense : The most basic type of estimate, which represents a type
 *       or class of purchases, planned expenditures, and in general represent
 *       a decrease to the total available funds.
 *   </li>
 *   <li>Income : The inverse of an expense, which represents an increase
 *       to the total available funds, such as wages or gifts.
 *   </li>
 *   <li>Transfer : A special type of estimate, which represents neither
 *       an increase or decrease to the total available funds, but rather
 *       a transfer of funds from one account to another.
 *   </li>
 * </ul>
 *
 * Estimates are structured in a tree-like hierarchy, where the root estimate
 * does not actually represent a usable estimate. That is, the root estimate
 * cannot have any money value associated with it. Estimates that are children
 * of the root can form sub-trees. All estimates within a given sub-tree are
 * of the same type of estimate. Like the root estimate, estimates that contain
 * children, or are parents of estimates, are considered estimate categories
 * and do not have a money value associated with them.
 *
 * For example,
 * <ul>
 *   <li>Root
 *     <ul>
 *       <li>Incomes (income type)
 *         <ul>
 *           <li>Mr's Salary (income type)</li>
 *           <li>Mrs's Salary (income type)</li>
 *         </ul>
 *       </li>
 *       <li>Expenses (expense type)
 *         <ul>
 *           <li>Living Expenses (expense type)
 *             <ul>
 *               <li>Rent (expense type)</li>
 *               <li>Utilities (expense type)
 *                 <ul>
 *                   <li>Electric (expense type)</li>
 *                   <li>Water (expense type)</li>
 *                   <li>Gas (expense type)</li>
 *               </li>
 *               <li>Food (expense type)</li>
 *               <li>Transportation (expense type)
 *                 <ul>
 *                   <li>Auto Insurance (expense type)</li>
 *                   <li>Gas (expense type)</li>
 *                 </ul>
 *               </li>
 *             </ul>
 *           </li>
 *           <li>Luxury Expenses (expense type)
 *             <ul>
 *               <li>Cell Phone (expense type)</li>
 *               <li>Entertainment (expense type)</li>
 *               <li>Dining Out (expense type)</li>
 *             </ul>
 *           </li>
 *         </ul>
 *       </li>
 *       <li>Credit Card Payment (transfer type)</li>
 *     </ul>
 *   </li>
 * </ul>
 *
 * When creating estimates, such as part of a retrieval from a persistent
 * storage medium, the two static factory methods should be used.
 * <ul>
 *   <li>`Estimate::createRoot()` is to be used to create an entirely new
 *       estimate tree. The new root is allocated on the heap and given as
 *       a shared pointer. When all references to an estimate root have
 *       gone out of scope, the entire tree of estimates will be destroyed.
 *       This is because an estimate is owned by its parent estimate, with
 *       the exception of the root estimate.
 *   </li>
 *   <li>`Estimate::create(...)` is to be used to create new estimates under
 *       a particular existing estimate. The new estimate is allocated on the
 *       heap and it's memory belongs to the parent estimate. The memory will
 *       be freed either when the parent estimate is deleted or when
 *       the estimate itself is deleted as a requested operation.
 *   </li>
 * </ul>
 *
 * All operations to an existing estimate are performed through the command
 * pattern. Requesting an operation, such as a name change or adding a new
 * child estimate, will return a `QUndoCommand` that is capable of applying
 * the operation and backing out the resulting changes.
 *
 * @ingroup estimate
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
	 * Estimate progress representation structure.
	 */
	struct Progress
	{
		/** Original estimated amount */
		Money estimated;
		/** Actual activity amount */
		Money actual;
		/** Optional note about progress */
		QString note;
		/** Health state of the estimate's progress */
		bool isHealthy;
	};

	/**
	 * Balance impact representation structure.
	 */
	struct Impact
	{
		/** Original estimated amount */
		Money estimated;
		/** Actual activity amount */
		Money actual;
		/** Expected ending amount */
		Money expected;
		/** Optional note about impact */
		QString note;
	};

	/**
	 * Creates a new root estimate for a new estimate tree. The new root
	 * estimate is allocated on the heap, and the memory is owned by the
	 * returned shared pointer. When all references to the root have gone
	 * out of scope, the `QSharedPointer` will delete the root estimate,
	 * and the entire tree of estimates along with it.
	 *
	 * @return shared pointer to the root estimate of a new estimate tree
	 */
	static QSharedPointer<Estimate> createRoot();

	/**
	 * Creates a new estimate as a child of the given estimate. The new
	 * estimate is allocated on the heap, and the memory is owned by the
	 * parent estimate. The estimate will be deleted automatically when
	 * the parent estimate is deleted and when the estimate is removed
	 * from the tree as a requested operation.
	 *
	 * This method for creating estimates should only be used when constructing
	 * an estimate tree from a persistent storage medium. Once constructed,
	 * new estimates should be added to the tree using `addChild()`.
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
	 * @param[in] offset      activity due date offset
	 * @param[in] finished    activity finished state
	 * @param[in] index       child index
	 */
	static Estimate* create(Estimate* parent,
		uint id, const QString& name, const QString& description, Type type,
		const Money& amount, int offset, bool finished, int index = -1);

	/**
	 * Destroys all children of this estimate.
	 */
	~Estimate();

	/**
	 * Returns the progress of this estimate as a comparison against
	 * actual activity against this estimate. Actual activity is given
	 * as a money value, which may be interpreted as money spent or
	 * earned depending on the type of this estimate. The actual money
	 * value will typically be a positive value, indicating a transfer
	 * of funds from one financial account to another.
	 *
	 * @param[in] actuals map of actual activity amounts
	 * @param[in] start   budgeting period start date
	 * @return progress of this estimate
	 */
	Progress progress(const QHash<uint,Money>& actuals,
		const QDate& start = QDate()) const;

	/**
	 * Returns the impact of this estimate on the estimated, actual,
	 * and expected ending balances.
	 *
	 * @param[in] actuals map of actual activity amounts
	 * @return impact of this estimate to ending balances
	 */
	Impact impact(const QHash<uint,Money>& actuals) const;

	// -- Modification methods (via command pattern)

	/**
	 * Creates an undoable command to change the name of
	 * this estimate.
	 *
	 * When this estimate's name is changed as a result of
	 * executing or undoing the created command, the `nameChanged()`
	 * signal is emitted.
	 *
	 * @param[in] newName new name for this estimate
	 * @param[in] cmd     parent command to be used for grouping
	 */
	QUndoCommand* changeName(const QString& newName,
		QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to change the description
	 * for this estimate.
	 *
	 * When this estimate's description is changed as a result
	 * of executing or undoing the created command, the
	 * `descriptionChanged()` signal is emitted.
	 *
	 * @param[in] newDescrip new description for this estimate
	 * @param[in] cmd        parent command to be used for grouping
	 */
	QUndoCommand* changeDescription(const QString& newDescrip,
		QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to change the type of this
	 * estimate, and the type of all of its child estimates.
	 * The type of an estimate dictates whether its estimated amount
	 * represents an expected increase or decrease in funds.
	 *
	 * When this estimate's type is changed as a result of executing
	 * or undoing the created command, the `typeChanged()` signal
	 * is emitted.
	 *
	 * @param[in] newType new type for the estimate tree for which
	 *                    this estimate is the parent
	 * @param[in] cmd     parent command to be used for grouping
	 */
	QUndoCommand* changeType(Type newType, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to change the estimated amount
	 * of this estimate. The estimated amount is an amount of
	 * expected increase or decrease in funds (depending on the type
	 * of this estimate) within the budgeting period.
	 *
	 * When this estimate's estimated amount is changed as a result
	 * of executing or undoing the created command, the
	 * `amountChanged()` signal is emitted.
	 *
	 * @param[in] newAmt new estimated amount for this estimate
	 * @param[in] cmd    parent command to be used for grouping
	 */
	QUndoCommand* changeAmount(const Money& newAmt,
		QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to set the due date offset for this
	 * estimate. A due date offset specifies the number of days after
	 * the budgeting period start date by which all activity should
	 * have occurred for an estimate. For example, a utility bill or
	 * credit card payment that is due by the 10th of every month.
	 * An offset of -1 clears the due date for this estimate.
	 *
	 * When this estimate's due date is changed as a result of
	 * executing or undoing the created command, the
	 * `dueDateOffsetChanged()` signal is emitted.
	 *
	 * @param[in] offset new due date offset for this estimate
	 * @param[in] cmd    parent command to be used for grouping
	 */
	QUndoCommand* changeDueDateOffset(int offset,
		QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to mark this estimate as finished.
	 * Finished estimates are estimates for which all expected activity
	 * has already occurred. This allows for their actual amounts to be
	 * used in calculating the expected ending balance.
	 *
	 * When this estimate's finished state is changed as a result of
	 * executing or undoing the created command, the
	 * `finishedStateChanged()` signal is emitted.
	 *
	 * @param[in] newState whether all activity for this estimate has finished
	 * @param[in] cmd      parent command to be used for grouping
	 */
	QUndoCommand* changeFinishedState(bool newState,
		QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to create a new child estimate
	 * under this estimate. The newly created child will be appended
	 * to the list of children for this estimate.
	 *
	 * When the child is created as a result of executing the created
	 * command, the `childAdded()` signal is emitted. When the child
	 * is destroyed as a result of undoing the created command, the
	 * `childRemoved()` and `deleted()` signals are emitted from their
	 * respective estimates.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* addChild(QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to delete this estimate from
	 * its parent and estimate tree. Upon execution of the command,
	 * the original estimate object will be deleted and its memory
	 * freed.
	 *
	 * When this estimate is deleted as a result of executing the
	 * created command, the `deleted()` signal is emitted prior to
	 * the estimate object being destroyed. A slot connected to this
	 * signal can continue to operate on the estimate until execution
	 * returns from the slot method. Additionally, the `childRemoved()`
	 * signal is emitted from this estimate's parent.
	 *
	 * When the estimate is recreated as a result of undoing the
	 * created command, the `childAdded()` signal is emitted from
	 * this estimate's parent.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* deleteEstimate(QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to move this estimate to a new
	 * parent at a specified index. If given the parent to which it already
	 * belongs, this estimate will be moved within the children of its parent.
	 *
	 * When this estimate is moved within the same parent as a result of
	 * executing or undoing the created command, the `childMoved()` signal
	 * is emitted from this estimate's parent.
	 *
	 * When this estimate is moved from one parent to another as a result of
	 * executing or undoing the created command, the `childRemoved()` signal
	 * is emitted from the old parent and the `childAdded()` signal is emitted
	 * from the new parent.
	 *
	 * @param[in] newParent new parent to which this estimate will be moved
	 * @param[in] newIndex  new index under the parent
	 * @param[in] cmd       parent command to be used for grouping
	 */
	QUndoCommand* moveTo(Estimate* newParent, int newIndex,
		QUndoCommand* cmd = 0);

	// -- Property access methods

	/**
	 * Returns the unique ID of this estimate. The estimate ID allows an
	 * estimate to be specified and retrieved from the tree with the
	 * `find()` method.
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
	 * Returns the type of this estimate. The estimate type dictates
	 * whether the estimate's estimated amount represents an expected
	 * increase or decrease in funds.
	 *
	 * @return type of this estimate
	 */
	Type estimateType() const;

	/**
	 * Returns the amount of this estimate. The estimated amount is an
	 * amount of expected increase or decrease in funds (depending on
	 * the type of the estimate) within the budgeting period.
	 *
	 * @return estimated amount of this estimate
	 */
	Money estimatedAmount() const;

	/**
	 * Returns the due date offset for this estimate. The due date offset
	 * specifies the number of days after the budgeting period start date
	 * by which all activity should have occurred for an estimate. For
	 * example, a utility bill or credit card payment that is due by the
	 * 10th of every month.
	 *
	 * An offset of -1 indicates that this estimate has no due date.
	 *
	 * @returns due date offset for this estimate
	 */
	int activityDueDateOffset() const;

	/**
	 * Returns whether activity has finished for this estimate.
	 * An estimate that is finished, or an estimate for which all activity
	 * against the stimate has occurred, can use the actual activity
	 * against the estimate to calculate the expecting ending balance.
	 *
	 * @return `true` if estimate activity has finished
	 */
	bool isActivityFinished() const;

	// -- Tree traversal methods

	/**
	 * Returns the parent estimate to this estimate. Ownership of the parent's
	 * memory remains with the parent of this estimate's parent.
	 *
	 * @return pointer to this estimate's parent estimate
	 */
	Estimate* parentEstimate() const;

	/**
	 * Returns the number of child estimates under this estimate.
	 *
	 * @return number of child estimates to this estimate
	 */
	int childCount() const;

	/**
	 * Returns the child estimate under this estimate at the given index.
	 * Ownership of the child's memory remains with this estimate (as parent).
	 *
	 * @return pointer to the child estimate at the given index
	 */
	Estimate* childAt(int index) const;

	/**
	 * Returns the child index of the given child estimate,
	 * if this estimate is a parent to that estimate.
	 *
	 * @return index of the given child index, or -1 if the estimate
	 *         is not a child of this estimate
	 */
	int indexOf(Estimate* child) const;

	/**
	 * Searches the estimate tree for the estimate with the given ID.
	 *
	 * @param[in] estimateId estimate ID to be searched
	 * @return estimate with the specified ID, or a null pointer if
	 *         no estimate exists with that ID
	 */
	Estimate* find(uint estimateId) const;

signals:
	/**
	 * Emitted when the name of this estimate changes as a result
	 * of a command created by `changeName()`.
	 *
	 * @param name new name of this estimate
	 */
	void nameChanged(const QString& name);

	/**
	 * Emitted when the description for this estimate changes as a
	 * result of a command created by `changeDescription()`.
	 *
	 * @param description new description for this estimate
	 */
	void descriptionChanged(const QString& description);

	/**
	 * Emitted when the type of this estimate changes as a result
	 * of a command created by `changeType()`.
	 *
	 * @param type new type of this estimate
	 */
	void typeChanged(Type type);

	/**
	 * Emitted when the estimated amount of this estimate changes
	 * as a result of a command created by `changeAmount()`.
	 *
	 * @param amount new estimated amount of this estimate
	 */
	void amountChanged(const Money& amount);

	/**
	 * Emitted when the due date offset for this estimate changes as a
	 * result of a command created by `changeDueDateOffset()`.
	 *
	 * @param offset new due date offset for this estimate
	 */
	void dueDateOffsetChanged(int offset);

	/**
	 * Emitted when the finished state of this estimate changes
	 * as a result of a command created by `changeFinishedState()`.
	 *
	 * @param finished new finished state for this estimate
	 */
	void finishedStateChanged(bool finished);

	/**
	 * Emitted when a child estimate is added to this estimate as
	 * a result of the commands created by either `addChild()`,
	 * `deleteEstimate()`, or `moveTo()`.
	 *
	 * @param child new child estimate that was added
	 * @param index index of the new child estimate
	 */
	void childAdded(Estimate* child, int index);

	/**
	 * Emitted when a child estimate is removed from this estimate as
	 * a result of the commands created by either `addChild()`,
	 * `deleteEstimate()`, or `moveTo()`.
	 *
	 * Note that the child estimate given in this signal does not
	 * necessarily have a valid parent estimate reference.
	 *
	 * @param child child estimate that was removed
	 * @param index old index of the child
	 */
	void childRemoved(Estimate* child, int index);

	/**
	 * Emitted when a child estimate is moved within the list of
	 * child estimates for this estimate as a result of the command
	 * created by `moveTo()`.
	 *
	 * @param child    child estimate that was moved
	 * @param oldIndex old index of the child estimate
	 * @param newIndex new index of the child estimate
	 */
	void childMoved(Estimate* child, int oldIndex, int newIndex);

	/**
	 * Emitted when this estimate is deleted as a result of the
	 * commands created by either `addChild()` or `deleteEstimate()`.
	 *
	 * After all slots have been executed, this estimate will be
	 * deleted, and it's memory freed. No more operations can be
	 * performed on this estimate object.
	 */
	void deleted();

private:
	/**
	 * Pointer to shared map of estimate paths. As a shared pointer,
	 * once all estimates have been deleted in an estimate tree, the
	 * map itself will also be destroyed.
	 *
	 * Each entry in the map consists of a list of indices, starting
	 * with the root estimate, necessary to reach a specific estimate.
	 *
	 * The map of paths is used to simplify searching for estimates
	 * by ID. The path entry for an estimate must be updated whenever
	 * the estimate is deleted or moved.
	 */
	QSharedPointer<QHash<uint, QList<int> > > paths;

	/** Parent estimate */
	Estimate* parent;
	/** Child estimates */
	QList<Estimate*> children;

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
	/** Due date offset for this estimate */
	int dueDateOffset;
	/** Finished state of this estimate */
	bool finished;

	/**
	 * Constructs a new root estimate. This constructor is private so that
	 * client code cannot create a root estimate on the stack.
	 */
	Estimate();

	/**
	 * Constructs a new estimate as a child of the given estimate. This
	 * constructor is private so that client code cannot create an estimate
	 * on the stack.
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
	 * @param[in] offset      activity due date offset
	 * @param[in] finished    activity finished state
	 * @param[in] index       child index
	 */
	Estimate(Estimate* parent, uint id, const QString& name,
		const QString& description, Type type, const Money& amount,
		int offset, bool finished, int index);

	/**
	 * Copy constructor is private to prevent copying. It is an invalid
	 * operation because all estimates are always allocated on the heap.
	 */
	Estimate(const Estimate& orig);

	/**
	 * Locate and return the pointer to the root estimate of the tree
	 * to which this estimate belongs.
	 *
	 * The root estimate is found by traversing up the tree via the
	 * parent estimate's until the root is found. If this estimate is
	 * the root estimate, a pointer to itself is returned.
	 *
	 * @return root estimate
	 */
	Estimate* root() const;

	/**
	 * Returns the estimate located at the specified path.
	 *
	 * Starting with the root estimate for the current tree, use each
	 * index entry in the path to traverse down the tree until
	 * the estimate is found. If at any point, an invalid index is
	 * specified (i.e., out of bounds) a null pointer is returned.
	 * If the path contains no index entries, a null pointer is returned.
	 *
	 * @param[in] path estimate ID path
	 * @return estimate at the specified path, or a null pointer if
	 *         the estimate was not found or the path was invalid
	 */
	Estimate* fromPath(const QList<int>& path) const;

	/**
	 * Returns the path of this estimate, as a list of child indices,
	 * starting with the root estimate.
	 *
	 * @return path to reach this estimate from the root estimate
	 */
	QList<int> path() const;

	/**
	 * Reinserts paths for all estimates under this estimate
	 * into the path map.
	 */
	void repath();

	/**
	 * Checks if this estimate is the root estimate.
	 *
	 * @return `true` if this estimate is the root
	 */
	bool isRoot() const;

	/**
	 * Checks if this estimate is a category of estimates, or if there
	 * are child estimates under this estimate.
	 *
	 * @return `true` if this estimate is a category of estimates
	 */
	bool isCategory() const;

	/**
	 * Retrieves the total estimated amount for all estimates that
	 * are descendants of this estimate, or the estimated amount
	 * of this estimate if it has no children.
	 *
	 * @return total estimated amount for all descendants
	 */
	Money totalEstimatedAmount() const;

	/**
	 * Retrieves the total actual amount for all estimates that
	 * are descendants of this estimate, or the actual amount
	 * of this estimate if it has no children.
	 *
	 * @param[in] actuals map of actual activity amounts
	 * @return total actual amount for all descendants
	 */
	Money totalActualAmount(const QHash<uint,Money>& actuals) const;

	/**
	 * Sets the name of this estimate, emitting the `nameChanged()`
	 * signal.
	 *
	 * @param[in] newName new name for this estimate
	 */
	void setName(const QString& newName);

	/**
	 * Sets the description for this estimate, emitting the
	 * `descriptionChanged()` signal.
	 *
	 * @param[in] newDescrip new description for this estimate
	 */
	void setDescription(const QString& newDescrip);

	/**
	 * Sets the type of this estimate, emitting the `typeChanged()`
	 * signal.
	 *
	 * @param[in] newType new type of this estimate
	 */
	void setType(Type newType);

	/**
	 * Sets the estimated amount of this estimate, emitting the
	 * `amountChanged()` signal.
	 *
	 * @param[in] newAmt new amount of this estimate
	 */
	void setAmount(const Money& newAmt);

	/**
	 * Sets the due date offset for this estimate, emitting the
	 * `dueDateOffsetChanged()` signal.
	 *
	 * @param[in] newOffset new due date offset for this estimate
	 */
	void setDueDateOffset(int newOffset);

	/**
	 * Marks this estimate as finished or unfinished, emitting
	 * the `finishedStateChanged()` signal.
	 *
	 * @param[in] newState whether this estimate is finished
	 */
	void setFinishedState(bool newState);

	/**
	 * Creates a new child estimate under this estimate, as the
	 * last child in this estimate's child list. The `childAdded()`
	 * signal is emitted.
	 *
	 * The parameters of the new estimate will be inherited
	 * from this estimate.
	 *
	 * @param[in] newId optional new ID of the child to be created
	 * @return ID of the newly created child estimate
	 */
	uint createChild(uint newId = 0);

	/**
	 * Creates a new child estimate under this estimate with
	 * the given parameters. The `childAdded()` signal is emitted.
	 *
	 * @param[in] id          estimate unique ID
	 * @param[in] name        estimate name
	 * @param[in] description estimate description
	 * @param[in] type        estimate type
	 * @param[in] amount      estimated amount
	 * @param[in] offset      activity due date offset
	 * @param[in] finished    activity finished state
	 * @param[in] index       index at which to insert the new estimate into
	 *                        the list of child estimates
	 */
	void createChild(uint id, const QString& name,
		const QString& description, Type type, const Money& amount,
		int offset, bool finished, int index = -1);

	/**
	 * Removes this estimate from the internal estimate pointer
	 * map and from its parent estimate. The `childRemoved()` signal
	 * is emitted from the parent estimate, and the `deleted()` signal
	 * is emitted from this estimate.
	 */
	void deleteSelf();

	/**
	 * Moves this estimate to the specified parent at the specified index.
	 * The `childAdded()`, `childRemoved()`, or `childMoved()` signals
	 * are emitted as needed.
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
	void addChild(Estimate* child, int index);

	/**
	 * Removes the given child from this estimate. Optionally,
	 * the child estimate object can be deleted and its memory
	 * freed as part of this operation. This should be done when
	 * an estimate is being completely removed from the estimate
	 * tree. If an estimate is merely being moved within the tree,
	 * it should not be deleted.
	 *
	 * Emits the childRemoved() signal.
	 *
	 * @param[in] child child estimate to be removed
	 * @param[in] del   whether to delete the estimate
	 */
	void removeChild(Estimate* child, bool del = false);

	/**
	 * Moves the given child to the given index.
	 *
	 * Emits the childMoved() signal.
	 *
	 * @param[in] child child estimate to be moved
	 * @param[in] index new index of the child estimate
	 */
	void moveChild(Estimate* child, int index);

	// Allow undoable commands private access
	friend class AddChildEstimateCommand;
	friend class ChangeEstimateAmountCommand;
	friend class ChangeEstimateDescriptionCommand;
	friend class ChangeEstimateDueDateOffsetCommand;
	friend class ChangeEstimateDueDateCommand;
	friend class ChangeEstimateFinishedCommand;
	friend class ChangeEstimateNameCommand;
	friend class ChangeEstimateTypeCommand;
	friend class DeleteEstimateCommand;
	friend class MoveEstimateCommand;
};

/**
 * Returns a string representation of the given estimate type.
 *
 * @param[in] type estimate type enumeration
 * @return string representation of the estimate type
 */
QString toString(Estimate::Type type);

}

// Make types known to Qt meta object system
Q_DECLARE_METATYPE(ub::Estimate::Type);

#endif // ESTIMATE_HPP
