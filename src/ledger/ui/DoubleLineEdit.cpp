/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "DoubleLineEdit.hpp"
#include "DoubleValidator.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
DoubleLineEdit::DoubleLineEdit(QWidget * parent)
        : QLineEdit(parent), validator_(new DoubleValidator(this)) {
    setValidator(validator_);
    connect(this, &QLineEdit::editingFinished, this, &DoubleLineEdit::finished);
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::clear() {
    QLineEdit::clear();
    current_ = 0.0;
}

//--------------------------------------------------------------------------------------------------
double DoubleLineEdit::value() const {
    return current_;
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::setValue(double value) {
    current_ = value;
    setText(QString::number(current_, 'f', 2));
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::focusOutEvent(QFocusEvent * event) {
    QLineEdit::focusOutEvent(event);
    emit focusLost();
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::finished() {
    current_ = validator_->evaluate(text());
    setText(QString::number(current_, 'f', 2));
}

} // ledger namespace
} // ub namespace
