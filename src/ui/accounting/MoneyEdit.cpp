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
#include "ui/accounting/MoneyEdit.hpp"
#include "ui/accounting/MoneyValidator.hpp"

namespace ub {

//------------------------------------------------------------------------------
MoneyEdit::MoneyEdit(QWidget* parent)
	: QLineEdit(parent)
{
	setup();
}

//------------------------------------------------------------------------------
MoneyEdit::MoneyEdit(const Money& contents, QWidget* parent)
	: QLineEdit(parent),
	  moneyValue(contents),
	  currentCurrency(contents.currency())
{
	setup();
}

//------------------------------------------------------------------------------
void MoneyEdit::setup()
{
	validator = new MoneyValidator(this);
	setValidator(validator);

	connect(this, SIGNAL(editingFinished()), this, SLOT(finished()));
	connect(this, SIGNAL(textEdited(QString)), this,
		SLOT(updateFromText(QString)));
}

//------------------------------------------------------------------------------
Money MoneyEdit::value() const
{
	return moneyValue;
}

//------------------------------------------------------------------------------
Currency MoneyEdit::currency() const
{
	return currentCurrency;
}

//------------------------------------------------------------------------------
void MoneyEdit::setValue(const Money& contents)
{
	if (moneyValue != contents)
	{
		moneyValue = contents;
		setText(moneyValue.toString());

		if (currentCurrency != contents.currency())
		{
			currentCurrency = contents.currency();
			validator->setCurrency(currentCurrency);
		}

		emit valueChanged(moneyValue);
	}
}

//------------------------------------------------------------------------------
void MoneyEdit::setCurrency(const Currency& currency)
{
	if (currentCurrency != currency)
	{
		currentCurrency = currency;
		validator->setCurrency(currency);

		moneyValue = moneyValue.to(currentCurrency);
		emit valueEdited(moneyValue);
	}
}

//------------------------------------------------------------------------------
void MoneyEdit::updateFromText(const QString& text)
{
	// Strip the currency symbol (if any) from the text
	QString asStr(text);
	asStr.replace(currentCurrency.symbol(), "");

	QLocale locale;
	bool ok;
	double asDbl = locale.toDouble(asStr, &ok);

	if (ok)
	{
		moneyValue = Money(asDbl, currentCurrency);
		emit valueEdited(moneyValue);
	}
}

//------------------------------------------------------------------------------
void MoneyEdit::finished()
{
	setText(moneyValue.toString());
}

}

