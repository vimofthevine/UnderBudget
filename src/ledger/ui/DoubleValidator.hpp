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
