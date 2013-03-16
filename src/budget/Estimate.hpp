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
#include "finance/Money.h"

// Forward declaration(s)
class Actual;
class QUndoCommand;

namespace ub::budget
{
	/**
	 * Representation of an estimated income or expense amount.
	 */
	class Estimate : public QObject
	{
	public:
		/**
		 * Estimate category type enumeration.
		 */
		enum Category
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
		};

		/**
		 * Constructs a new root estimate.
		 *
		 * @param[in] uid estimate unique ID
		 */
		Estimate(const QUuid& uid);

		/**
		 * Constructs a new estimate.
		 *
		 * @param[in] parent      parent estimate
		 * @param[in] uid         estimate unique ID
		 * @param[in] name        estimate name
		 * @param[in] description estimate description
		 * @param[in] category    estimate category
		 * @param[in] amount      estimated amount
		 * @param[in] dueDate     due date
		 * @param[in] complete    whether activity is complete
		 */
		Estimate(QSharedPointer<Estimate> parent, const QUuid& uid,
			const QString& name, const QString& description,
			Category category, const Money& amount,
			const QDate& dueDate, bool complete);

		/**
		 * Constructs an estimate from an existing estimate
		 *
		 * @param[in] orig original estimate
		 */
		Estimate(const Estimate& orig);

		/**
		 * Destructs this estimate.
		 */
		~Estimate();

		/**
		 * Returns the progress of this estimate as compared
		 * to the given actual figure.
		 *
		 * @param actual actual figure to be compared against
		 * @return progress of this estimate
		 */
		Progress getProgress(const Actual& actual) const;

		/**
		 * Returns the impact of this estimate to the ending
		 * balances as determined with the given actual figure.
		 *
		 * @param actual actual figure to be used
		 * @return impact of this estimate to ending balances
		 */
		Impact getImpact(const Actual& actual) const;

		// -- Modification methods (via command pattern)

		/**
		 * Creates a command to change the name of this estimate.
		 *
		 * @param[in] newName new name for the estimate
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* changeName(const QString& newName,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to change the description for
		 * this estimate.
		 *
		 * @param[in] newDescrip new description for the estimate
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* changeDescription(const QString& newDescrip,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to change the category of this
		 * estimate, and all its child estimates.
		 *
		 * @param[in] newCat new category for the estimate tree
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* changeCategory(Category newCat,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to change the amount of this
		 * estimate.
		 *
		 * @param[in] newAmt new estimated amount for this estimate
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand** changeAmount(const Money& newAmt,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to set the due date for this
		 * estimate. A due date specifies a date by which
		 * activity should have occurred for an estimate.
		 * For example, a utility bill or credit card
		 * payment that is due by a particular date.
		 *
		 * @param[in] newDate new due date for this estimate
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* changeDueDate(const QDate& newDate,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to mark this estimate as
		 * complete. Complete estimates are estimates
		 * for which all expected activity has already
		 * occurred. This allows for their actual
		 * amounts to be used in calculating the
		 * expected ending balance.
		 *
		 * @param[in] isComplete whether the estimate is complete
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* changeComplete(bool isComplete,
			QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to create a new child estimate
		 * to this estimate.
		 *
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* addChild(QUndoCommand* parent = 0) const;

		/**
		 * Creates a command to delete this estimate.
		 *
		 * @param[in] parent  parent command to be used for grouping
		 */
		QUndoCommand* delete(QUndoCommand* parent = 0) const;

		// -- Tree traversal methods

		/**
		 * Returns the parent estimate for this estimate.
		 *
		 * @return parent estimate
		 */
		QSharedPointer<Estimate> parent() const;

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
		 * This signal is emitted whenever the category of this
		 * estimate changes.
		 *
		 * @param category current category of this estimate
		 */
		void categoryChanged(Category category);

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
		 * is marked as complete or not complete.
		 *
		 * @param complete whether this estimate is complete
		 */
		void completeChanged(bool complete);

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
		/** Unique ID */
		const QUuid myUid;
		/** Name of this estimate */
		QString myName;
		/** Description for this estimate */
		QString myDescription;
		/** Category of this estimate */
		Category myCategory;
		/** Amount of this estimate */
		Money myAmount;
		/** Due date for this estimate */
		QDate myDueDate;
		/** Whether this estimate is complete */
		bool myComplete;
		/** Parent estimate */
		QSharedPointer<Estimate> myParent;
		/** Child estimates */
		QList<QSharedPointer<Estimate> > myChildren;

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
		 * Sets the category of this estimate.
		 *
		 * @param[in] newCat new category of this estimate
		 */
		void setCategory(Category newCat);

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
		 * Marks this estimate as complete or not complete.
		 *
		 * @param[in] isComplete whether this estimate is complete
		 */
		void setComplete(bool isComplete);

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
		 * @param[in] category    estimate category
		 * @param[in] amount      estimated amount
		 * @param[in] dueDate     due date
		 * @param[in] complete    whether activity is complete
		 */
		QSharedPointer<Estimate> createChild(const QUuid& uid,
			const QString& name, const QString& description,
			Category category, const Money& amount,
			const QDate& dueDate, bool complete);

		/**
		 * Removes this estimate from its parent estimate.
		 * This estimate should be deleted after this
		 * method is called.
		 */
		void removeFromParent();
	};
}

#endif // ESTIMATE_HPP
