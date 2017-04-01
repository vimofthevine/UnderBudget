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

#pragma once

// Qt include(s)
#include <QChar>
#include <QList>
#include <QString>
#include <QValidator>

// Forward declaration(s)
class QObject;
class QScriptEngine;

namespace ub {
namespace ledger {

/**
 * Regular expression validator for editors for double values.
 */
class DoubleValidator : public QValidator {
    Q_OBJECT

public:
    /**
     * Initializes the validator.
     *
     * @param parent Parent object
     */
    DoubleValidator(QObject * parent = 0);

    /**
     * Evaluates the given input.
     *
     * @param input String input
     * @return Numberic value from the input
     */
    double evaluate(const QString & input) const;

    State validate(QString & input, int & pos) const override;

private:
    /** Allowed intermediate characters */
    QList<QChar> allowed_;
    /** Script engine */
    QScriptEngine * engine_;
};

} // ledger namespace
} // ub namespace
