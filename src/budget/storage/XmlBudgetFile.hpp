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

#ifndef XMLBUDGETFILE_HPP
#define XMLBUDGETFILE_HPP

// Qt include(s)

// UnderBudget include(s)
#include "budget/storage/BudgetSource.hpp"

namespace ub {

/**
 * A budget source for budgets stored in an XML file.
 */
class XmlBudgetFile : public BudgetSource {
public:
	/**
	 * Constructs an XML budget file source for the specified file path.
	 *
	 * @param[in] fileName XML budget file location
	 */
	XmlBudgetFile(const QString& fileName);

	// Implemented base methods
	QSharedPointer<Budget> retrieve();
	bool store(QSharedPointer<Budget> budget);
	QString error() const;
	QString location() const;

private:
	/** XML budget file name */
	const QString xmlFile;
	/** Last error message */
	QString errorMsg;
};

}

#endif //XMLBUDGETFILE_HPP

