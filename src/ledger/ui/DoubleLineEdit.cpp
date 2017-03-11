/*
 * Copyright 2017 Kyle Treubig
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
