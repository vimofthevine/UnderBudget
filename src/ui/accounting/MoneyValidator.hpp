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

// Qt include(s)
#include <QRegExpValidator>

// UnderBudget include(s)
#include "accounting/Currency.hpp"

namespace ub {

/**
 * Custom input validator for money editor text input fields.
 *
 * @ingroup ui_accounting
 */
class MoneyValidator : public QRegExpValidator
{
	Q_OBJECT

public:
	/**
	 * Constructs a new money input validator.
	 *
	 * @param[in] parent parent object
	 */
	MoneyValidator(QObject* parent = 0);

	/**
	 * Specifies the currency of the money values being validated.
	 *
	 * @param[in] currency new currency of money values to be validated
	 */
	void setCurrency(const Currency& currency);

	/**
	 * Attempts to correctly form the input text as a valid
	 * money value.
	 *
	 * @param[in,out] input user input text to be fixed up
	 */
	void fixup(QString& input) const;

private:
	/** Currency symbol */
	QString symbol;
};

}

