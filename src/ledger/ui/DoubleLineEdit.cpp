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
    setText(QString::number(current_));
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::focusOutEvent(QFocusEvent * event) {
    QLineEdit::focusOutEvent(event);
    emit focusLost();
}

//--------------------------------------------------------------------------------------------------
void DoubleLineEdit::finished() {
    current_ = validator_->evaluate(text());
    setText(QString::asprintf("%.2f", current_));
}

} // ledger namespace
} // ub namespace
