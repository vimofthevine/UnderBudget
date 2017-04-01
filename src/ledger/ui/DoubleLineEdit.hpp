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
