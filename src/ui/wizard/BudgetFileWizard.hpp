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

#ifndef BUDGETFILEWIZARD_HPP
#define BUDGETFILEWIZARD_HPP

// Qt include(s)
#include <QString>

// UnderBudget include(s)

// Forward declaration(s)
class QWidget;

namespace ub {

/**
 * Budget source file selection wizard.
 */
class BudgetFileWizard {

public:
	static QString promptForFileToOpen(QWidget* parent);

	static QString promptForFileToSave(QWidget* parent, const QString& existing);

private:
	// Last-used budget file directory settings key
	static const QString LAST_USED_BUDGET_DIR;
};

}

#endif //BUDGETFILEWIZARD_HPP
