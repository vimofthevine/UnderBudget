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
#include <QtScript/QScriptEngine>
#include <QtWidgets>

// UnderBudget include(s)
#include "DoubleValidator.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
DoubleValidator::DoubleValidator(QObject * parent)
        : QValidator(parent), engine_(new QScriptEngine(this)) {
    allowed_ << '.' << ' ' << '(' << ')' << '+' << '-' << '*' << '/';
}

//--------------------------------------------------------------------------------------------------
double DoubleValidator::evaluate(const QString & input) const {
    auto result = engine_->evaluate(input);
    if (result.isNumber()) {
        return result.toNumber();
    } else {
        return 0.0;
    }
}

//--------------------------------------------------------------------------------------------------
DoubleValidator::State DoubleValidator::validate(QString & input, int & pos) const {
    if (input.isEmpty()) {
        return Intermediate;
    }
    auto result = engine_->evaluate(input);
    if (result.isNumber()) {
        return Acceptable;
    } else {
        auto last = input.at(input.length() - 1);
        if (last.isDigit() or (allowed_.contains(last))) {
            return Intermediate;
        } else {
            return Invalid;
        }
    }
}

} // ledger namespace
} // ub namespace
