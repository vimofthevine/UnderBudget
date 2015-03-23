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

#ifndef BUDGETSOURCE_HPP
#define BUDGETSOURCE_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QString>

// UnderBudget include(s)
#include "budget/Budget.hpp"

namespace ub {

/**
 * Abstract budget source class. Concrete implementations may read or write
 * budgets to and from files, databases, network locations, etc.
 *
 * @ingroup budget_storage
 */
class BudgetSource {
public:
	/** Virtual destructor */
	virtual ~BudgetSource() { }

	/**
	 * Retrieves the budget defined by this budget source. If an error occurs
	 * in retrieving the budget, the returned pointer will be null. The error
	 * message can be retrieved by `error()`.
	 *
	 * @return retrieved budget
	 */
	virtual QSharedPointer<Budget> retrieve() = 0;

	/**
	 * Stores the given budget in this budget source. If an error occurs
	 * in storing the budget, the error message can be retrieved by `error()`.
	 *
	 * @return `true` if the budget was successfully stored
	 */
	virtual bool store(QSharedPointer<Budget> budget) = 0;

	/**
	 * Returns the error message from the last executed action. If no error
	 * occurred, the returned string will be empty.
	 *
	 * @return last error message
	 */
	virtual QString error() const = 0;

	/**
	 * Returns a description of this source's location. It could be a file name,
	 * a database name, a remote network location, etc.
	 *
	 * @return source location
	 */
	virtual QString location() const = 0;
};

}

#endif //BUDGETSOURCE_HPP
