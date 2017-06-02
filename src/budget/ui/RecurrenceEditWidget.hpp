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
#include <QWidget>

// UnderBudget include(s)
#include <budget/model/Recurrence.hpp>

// Forward declaration(s)
class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;

namespace ub {
namespace budget {

/**
 * User input widget for modifying budget recurrences.
 *
 * @ingroup budget
 */
class RecurrenceEditWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * Initializes the recurrence editor widget.
     *
     * @param parent Parent widget
     */
    RecurrenceEditWidget(QWidget * parent);

    /**
     * Returns the recurrence value defined by the widget fields.
     *
     * @return Recurrence value
     */
    Recurrence recurrence() const;

    /**
     * Clears and resets the widget fields.
     */
    void clear();

signals:
    /**
     * Emitted when the user has enabled recurrence.
     *
     * @param recurring @c true if recurrence has been selected
     */
    void isRecurring(bool recurring);

public slots:
    /**
     * Updates the contents of the widget to represent the given recurrence value.
     *
     * @param recurrence Recurrence value
     */
    void setRecurrence(const Recurrence & recurrence);

private slots:
    /**
     * Updates displayed fields based on the new scope selection.
     *
     * @param index New scope selection index
     */
    void updateFields(int index);

private:
    /** One-time or recurring checkbox */
    QCheckBox * recurring_;
    /** Periodicity spinner */
    QSpinBox * periodicity_;
    /** Scope selection */
    QComboBox * scope_;
    /** Month selection */
    QComboBox * month_;
    /** Week spinner */
    QSpinBox * week_;
    /** Day spinner */
    QSpinBox * day_;
    /** Checkbox to enable month input */
    QCheckBox * enable_month_;
    /** Checkbox to enable week input */
    QCheckBox * enable_week_;
    /** Checkbox to enable day input */
    QCheckBox * enable_day_;
};

} // budget namespace
} // ub namespace
