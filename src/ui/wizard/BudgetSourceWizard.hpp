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

#ifndef BUDGETSOURCEWIZARD_HPP
#define BUDGETSOURCEWIZARD_HPP

// Qt include(s)
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/storage/BudgetSource.hpp"

// Forward declaration(s)
class QWidget;

namespace ub {

/**
 * Budget source selection wizard.
 *
 * @ingroup ui_wizard
 */
class BudgetSourceWizard {
public:
	/**
	 * Prompts the user for all information necessary to re-open a budget
	 * from the specified location. If the described budget source could
	 * not be determined, the returned pointer will be null.
	 *
	 * @param[in] parent   parent widget
	 * @param[in] location budget location description
	 * @return budget source from which to retrieve an existing budget
	 */
	static QSharedPointer<BudgetSource> promptToReOpen(QWidget* parent,
		const QString& location);

	/**
	 * Prompts the user for all information necessary to open a budget
	 * from the user-specified source. If the user cancels the operation or
	 * the selected source is invalid, the returned pointer will be null.
	 *
	 * @param[in] parent parent widget
	 * @return budget source from which to retrieve an existing budget
	 */
	static QSharedPointer<BudgetSource> promptForBudgetToOpen(QWidget* parent);

	/**
	 * Prompts the user for all information necessary to save a budget
	 * to the user-specified source. If the user cancels the operation or
	 * the selected source is invalid, the returned pointer will be null.
	 *
	 * @param[in] parent   parent widget
	 * @param[in] existing original budget source, can be a null pointer
	 * @return budget source to which to save a budget
	 */
	static QSharedPointer<BudgetSource> promptForBudgetToSave(QWidget* parent,
		QSharedPointer<BudgetSource> existing);

private:
	// Last-used budget file directory settings key
	static const QString LAST_USED_BUDGET_DIR;

	/**
	 * Records the directory containing the specified file as the last-used
	 * budget file directory. This allows the next open operation to begin
	 * in the same directory.
	 *
	 * @param[in] fileName name of a budget file
	 */
	static void recordLastUsedPath(const QString& fileName);

	/**
	 * Creates a budget source for the specified file name. The type of the
	 * budget source created depends on the extension of the file name. If
	 * the file extension is unrecognized, a null pointer is returned.
	 *
	 * @param[in] fileName name of a budget file
	 * @return budget source for the specified budget file, or a null pointer
	 *         if the specified file name contains an unrecognized extension
	 */
	static BudgetSource* createForFile(const QString& fileName);
};

}

#endif //BUDGETSOURCEWIZARD_HPP
