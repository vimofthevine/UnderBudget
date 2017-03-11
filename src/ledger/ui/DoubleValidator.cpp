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
