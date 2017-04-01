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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <budget/model/Recurrence.hpp>
#include "RecurrenceEditWidget.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
RecurrenceEditWidget::RecurrenceEditWidget(QWidget * parent)
        : QWidget(parent), recurring_(new QCheckBox(tr("Recurring?"), this)),
          periodicity_(new QSpinBox(this)), scope_(new QComboBox(this)),
          month_(new QComboBox(this)), week_(new QSpinBox(this)),
          day_(new QSpinBox(this)), enable_month_(new QCheckBox(tr("Specific month?"), this)),
          enable_week_(new QCheckBox(tr("Specific week?"), this)),
          enable_day_(new QCheckBox(tr("Specific day?"), this)) {
    periodicity_->setMinimum(1);
    periodicity_->setMaximum(52);

    scope_->addItem(tr("Week(s)"), QVariant::fromValue(Recurrence::Weekly));
    scope_->addItem(tr("Month(s)"), QVariant::fromValue(Recurrence::Monthly));
    scope_->addItem(tr("Year(s)"), QVariant::fromValue(Recurrence::Yearly));
    connect(scope_, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &RecurrenceEditWidget::updateFields);

    QLocale locale;
    for (int i = 1; i < 13; ++i) {
        month_->addItem(locale.monthName(i), i);
    }

    week_->setMinimum(1);
    week_->setMaximum(52);

    day_->setMinimum(1);
    day_->setMaximum(7);

    QHBoxLayout * fields = new QHBoxLayout;
    fields->addWidget(new QLabel(tr("Every")));
    fields->addWidget(periodicity_);
    fields->addWidget(scope_);

    auto month_label = new QLabel(tr("in"));
    month_label->setVisible(false);
    month_->setVisible(false);
    enable_month_->setVisible(false);
    connect(enable_month_, &QCheckBox::toggled, month_label, &QLabel::setVisible);
    connect(enable_month_, &QCheckBox::toggled, month_, &QComboBox::setVisible);
    fields->addWidget(month_label);
    fields->addWidget(month_);

    auto week_label = new QLabel(tr("in week"));
    week_label->setVisible(false);
    week_->setVisible(false);
    enable_week_->setVisible(false);
    connect(enable_week_, &QCheckBox::toggled, week_label, &QLabel::setVisible);
    connect(enable_week_, &QCheckBox::toggled, week_, &QSpinBox::setVisible);
    fields->addWidget(week_label);
    fields->addWidget(week_);

    auto day_label = new QLabel(tr("on day"));
    day_label->setVisible(false);
    day_->setVisible(false);
    connect(enable_day_, &QCheckBox::toggled, day_label, &QLabel::setVisible);
    connect(enable_day_, &QCheckBox::toggled, day_, &QSpinBox::setVisible);
    fields->addWidget(day_label);
    fields->addWidget(day_);

    QHBoxLayout * checkboxes = new QHBoxLayout;
    checkboxes->addWidget(enable_month_);
    checkboxes->addWidget(enable_week_);
    checkboxes->addWidget(enable_day_);

    QVBoxLayout * recurring_layout = new QVBoxLayout;
    recurring_layout->addItem(fields);
    recurring_layout->addItem(checkboxes);

    QWidget * recurring_widget = new QWidget(this);
    recurring_widget->setLayout(recurring_layout);
    recurring_widget->setVisible(false);
    connect(recurring_, &QCheckBox::toggled, recurring_widget, &QWidget::setVisible);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(recurring_);
    layout->addWidget(recurring_widget);
    setLayout(layout);

    clear();
}

//--------------------------------------------------------------------------------------------------
Recurrence RecurrenceEditWidget::recurrence() const {
    Recurrence rec;

    if (recurring_->isChecked()) {
        rec.setPeriodicity(periodicity_->value());
        rec.setScope(scope_->currentData().value<Recurrence::ScopeType>());
        if (enable_month_->isChecked()) {
            rec.setMonth(month_->currentIndex() + 1);
        }
        if (enable_week_->isChecked()) {
            rec.setWeek(week_->value());
        }
        if (enable_day_->isChecked()) {
            rec.setDay(day_->value());
        }
    }

    return rec;
}

//--------------------------------------------------------------------------------------------------
void RecurrenceEditWidget::clear() {
    recurring_->setChecked(false);
    periodicity_->setValue(1);
    scope_->setCurrentIndex(0);
    month_->setCurrentIndex(0);
    week_->setValue(1);
    day_->setValue(1);
    enable_month_->setChecked(false);
    enable_week_->setChecked(false);
    enable_day_->setChecked(false);
}

//--------------------------------------------------------------------------------------------------
void RecurrenceEditWidget::setRecurrence(const Recurrence & recurrence) {
    clear();
    if (recurrence.periodicity() > 0) {
        recurring_->setChecked(true);
        periodicity_->setValue(recurrence.periodicity());
        switch (recurrence.scope()) {
        case Recurrence::Weekly:
            scope_->setCurrentIndex(0);
            break;
        case Recurrence::Monthly:
            scope_->setCurrentIndex(1);
            break;
        case Recurrence::Yearly:
            scope_->setCurrentIndex(2);
            break;
        default:
            scope_->setCurrentIndex(0);
        }
        if (recurrence.month() > 0) {
            month_->setCurrentIndex(recurrence.month() - 1);
            enable_month_->setChecked(true);
        }
        if (recurrence.week() > 0) {
            week_->setValue(recurrence.week());
            enable_week_->setChecked(true);
        }
        if (recurrence.day() > 0) {
            day_->setValue(recurrence.day());
            enable_day_->setChecked(true);
        }
    }
}

//--------------------------------------------------------------------------------------------------
void RecurrenceEditWidget::updateFields(int index) {
    switch (index) {
    case 0: // Weekly
        enable_month_->setChecked(false);
        enable_month_->setVisible(false);
        enable_week_->setChecked(false);
        enable_week_->setVisible(false);
        day_->setMaximum(7);
        break;
    case 1: // Monthly
        enable_month_->setChecked(false);
        enable_month_->setVisible(false);
        enable_week_->setVisible(true);
        week_->setMaximum(4);
        day_->setMaximum(31);
        break;
    case 2: // Yearly
        enable_month_->setVisible(true);
        enable_week_->setVisible(true);
        week_->setMaximum(52);
        day_->setMaximum(365);
        break;
    default:
        break;
    }
}

} // budget namespace
} // ub namespace
