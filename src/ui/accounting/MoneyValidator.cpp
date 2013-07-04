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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/accounting/MoneyValidator.hpp"

namespace ub {

//------------------------------------------------------------------------------
MoneyValidator::MoneyValidator(QObject* parent)
	: QRegExpValidator(parent)
{
	setCurrency(Currency());
}

//------------------------------------------------------------------------------
void MoneyValidator::setCurrency(const Currency& currency)
{
	symbol = currency.symbol();

	// Escape every character in symbol
	QString escaped(symbol);
	for (int i=symbol.length(); i>0; --i)
	{
		escaped.insert(i-1, "\\");
	}

	QLocale locale;
	QRegExp regex("(" + escaped + ")?"
		+ "[\\d]{1,3}"
		+ "(,?[\\d]{3})*"
		+ "\\" + locale.decimalPoint()
		+ "[\\d]{2}");
	setRegExp(regex);
}

//------------------------------------------------------------------------------
void MoneyValidator::fixup(QString& input) const
{
	if (input == "")
	{
		input = symbol + "0.00";
	}

	if ( ! input.startsWith(symbol))
	{
		input.prepend(symbol);
	}

	if ( ! input.contains("."))
	{
		input.append(".00");
	}
}

}

