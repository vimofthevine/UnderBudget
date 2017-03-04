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
