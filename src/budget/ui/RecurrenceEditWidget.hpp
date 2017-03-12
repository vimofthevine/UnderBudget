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
