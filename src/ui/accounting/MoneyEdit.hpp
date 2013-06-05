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

#ifndef MONEYEDIT_HPP
#define MONEYEDIT_HPP

// Qt include(s)
#include <QLineEdit>

// UnderBudget include(s)
#include "accounting/Money.hpp"

namespace ub {

// Forward declaration(s)
class MoneyValidator;

/**
 * A text editor for entering monetary values.
 */
class MoneyEdit : public QLineEdit
{
	Q_OBJECT

public:
	/**
	 * Constructs a money editor with a zero value.
	 *
	 * @param[in] parent parent widget
	 */
	MoneyEdit(QWidget* parent = 0);

	/**
	 * Constructs a money editor with the given value.
	 *
	 * @param[in] contents initial money value to be displayed
	 * @param[in] parent   parent widget
	 */
	MoneyEdit(const Money& contents, QWidget* parent = 0);

	/**
	 * Retrieves the current money value represented in the editor.
	 *
	 * @return money value represented by the editor
	 */
	Money value() const;

	/**
	 * Retrieves the current currency used for all values entered
	 * into the editor.
	 *
	 * @return currency used for all entered money values
	 */
	Currency currency() const;

public slots:
	/**
	 * Sets the money value represented in the editor.
	 *
	 * @param[in] contents new money value to be displayed
	 */
	void setValue(const Money& contents);

	/**
	 * Sets the currency that will be used for all money values
	 * entered into the editor. The money value currently represented
	 * in the editor will be converted to the new currency and an
	 * valueEdited() signal will be emitted.
	 *
	 * @param[in] currency new currency to be used for all values
	 */
	void setCurrency(const Currency& currency);

private slots:
	/**
	 * Updates the money value specified by the user input text.
	 * This slot is connected to the QLineEdit's textEdited() signal so it
	 * is updated whenever the user updates the editor value.
	 *
	 * @param[in] text user input text
	 */
	void updateFromText(const QString& text);

	/**
	 * Updates the displayed text with that of the money value's toString()
	 * representation when editing has finished.
	 */
	void finished();

signals:
	/**
	 * This signal is emitted whenever the value is changed. The money argument
	 * is the new value. Unlike valueEdited(), this signal is emitted when the
	 * value is changed programmatically, for example, by calling setValue().
	 */
	void valueChanged(const Money& value);

	/**
	 * This signal is emitted whenever the value is edited. The money argument is
	 * the new value.  Unlike valueChanged(), this signal is not emitted when the
	 * value is changed programmatically, for example, by calling setValue().
	 */
	void valueEdited(const Money& value);

private:
	/** Money input validator */
	MoneyValidator* validator;
	/** The current money value in the editor */
	Money moneyValue;
	/** The current currency used for all money values entered into the editor */
	Currency currentCurrency;

	/**
	 * Sets up the validator and connects all internal signals and slots.
	 */
	void setup();
};

}

#endif //MONEYEDIT_HPP
