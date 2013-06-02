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

#ifndef BUDGET_HPP
#define BUDGET_HPP

// Qt include(s)
#include <QObject>

// Forward declaration(s)
class QUndoCommand;

namespace ub {

class Budget : public QObject {
	Q_OBJECT

public:
	/**
	 * Constructs a new, unpopulated budget.
	 */
	Budget();

	~Budget() { }

	/**
	 * Returns the user-defined name for this budget.
	 *
	 * @return budget name
	 */
	QString name() const;

	/**
	 * Creates an undoable command to change the budget's name.
	 * Ownership of the returned pointer is transfered to the
	 * caller of this function.
	 *
	 * @param[in] newName new budget name
	 * @return undoable command to apply the name change
	 */
	QUndoCommand* changeName(const QString& newName);

signals:
	/**
	 * Emitted when the name of the budget is changed.
	 *
	 * @param name new budget name
	 */
	void nameChanged(const QString& name);

private:
	/** User-defined name */
	QString budgetName;

	/**
	 * Sets the budget's name to the given name, emitting
	 * the `nameChanged` signal.
	 *
	 * @param[in] name new budget name
	 */
	void setName(const QString& name);

	// Allow undoable commands direct field access
	friend class ChangeBudgetNameCommand;
};

}

#endif //BUDGET_HPP
