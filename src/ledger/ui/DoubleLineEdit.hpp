#pragma once

// Qt include(s)
#include <QLineEdit>

// Forward declaration(s)
class QFocusEvent;
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class DoubleValidator;

/**
 * Line edit widget customized to handle arithmetic expressions generating double values.
 *
 * @ingroup ledger
 */
class DoubleLineEdit : public QLineEdit {
    Q_OBJECT

public:
    /**
     * Initializes the double line edit widget.
     *
     * @param parent Parent widget
     */
    DoubleLineEdit(QWidget * parent = 0);

    /**
     * Returns the double value represented in the line edit widget.
     *
     * @return Double value of the line edit widget
     */
    double value() const;

    /**
     * Clears the widget.
     */
    void clear();

signals:
    /** Emitted when focus has been removed from the double line edit widget. */
    void focusLost();

protected:
    void focusOutEvent(QFocusEvent * event) override;

public slots:
    /**
     * Updates the contents of the line edit widget to contain the given double value.
     *
     * @param value Double value
     */
    void setValue(double value);

private slots:
    void finished();

private:
    /** Validator */
    DoubleValidator * validator_;
    /** Current value */
    double current_;
};

} // ledger namespace
} // ub namespace
