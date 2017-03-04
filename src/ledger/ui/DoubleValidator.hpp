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
