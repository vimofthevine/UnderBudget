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
#include <budget/model/Income.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/ui/DoubleLineEdit.hpp>
#include "IncomeModel.hpp"
#include "IncomeDetailsDialog.hpp"
#include "RecurrenceEditWidget.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
IncomeDetailsDialog::IncomeDetailsDialog(IncomeModel * model, QWidget * parent)
        : QDialog(parent), model_(model), description_(new QLineEdit(this)),
          beginning_date_(new QDateEdit(this)), ending_date_(new QDateEdit(this)),
          recurrence_(new RecurrenceEditWidget(this)), amount_(new ledger::DoubleLineEdit(this)),
          buttons_(new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset |
                                        QDialogButtonBox::Cancel)) {
    description_->setPlaceholderText(tr("Income description"));
    beginning_date_->setCalendarPopup(true);
    ending_date_->setCalendarPopup(true);
    ending_date_->setSpecialValueText(tr("Never"));

    connect(buttons_, &QDialogButtonBox::clicked, this, &IncomeDetailsDialog::clicked);

    auto start_today = new QPushButton(tr("Today"), this);
    connect(start_today, &QPushButton::clicked, this, [this] () {
        beginning_date_->setDate(QDate::currentDate());
    });

    QHBoxLayout * start = new QHBoxLayout;
    start->addWidget(beginning_date_);
    start->addWidget(start_today);

    auto stop_today = new QPushButton(tr("Today"), this);
    connect(stop_today, &QPushButton::clicked, this, [this] () {
        ending_date_->setDate(QDate::currentDate());
    });

    auto stop_never = new QPushButton(tr("Never"), this);
    connect(stop_never, &QPushButton::clicked, this, [this] () {
        ending_date_->setDate(ending_date_->minimumDate());
    });

    QHBoxLayout * stop = new QHBoxLayout;
    stop->addWidget(ending_date_);
    stop->addWidget(stop_today);
    stop->addWidget(stop_never);

    QFormLayout * form = new QFormLayout;
    form->addRow(tr("Description"), description_);
    form->addRow(tr("Starting Date"), start);
    form->addRow(tr("Ending Date"), stop);
    form->addRow(tr("Recurrence"), recurrence_);
    form->addRow(tr("Amount"), amount_);
    form->addRow("", buttons_);
    setLayout(form);

    setWindowTitle(tr("Budgeted Income"));
}

//--------------------------------------------------------------------------------------------------
void IncomeDetailsDialog::resetForNewIncome(const ledger::Account & account) {
    index_ = QModelIndex();
    income_ = Income();
    income_.setAccount(account);
    description_->clear();
    beginning_date_->setDate(QDate::currentDate());
    ending_date_->setDate(ending_date_->minimumDate());
    recurrence_->clear();
    amount_->clear();
    show();
}

//--------------------------------------------------------------------------------------------------
void IncomeDetailsDialog::showIncome(const QModelIndex & index) {
    index_ = index;
    income_ = model_->income(index);
    description_->setText(income_.description());
    beginning_date_->setDate(income_.beginningDate());
    if (income_.endingDate().isValid()) {
        ending_date_->setDate(income_.endingDate());
    } else {
        ending_date_->setDate(ending_date_->minimumDate());
    }
    recurrence_->setRecurrence(income_.recurrence());
    amount_->setValue(income_.amount().amount());
    show();
}

//--------------------------------------------------------------------------------------------------
void IncomeDetailsDialog::duplicateIncome(const QModelIndex & index) {
    showIncome(index);
    index_ = QModelIndex();
}

//--------------------------------------------------------------------------------------------------
void IncomeDetailsDialog::clicked(QAbstractButton * button) {
    if (button == buttons_->button(QDialogButtonBox::Save)) {
        Income income = income_;
        income.setAmount(ledger::Money(amount_->value(), income.account().currency()));
        income.setBeginningDate(beginning_date_->date());
        income.setDescription(description_->text());
        if (ending_date_->date() == ending_date_->minimumDate()) {
            income.setEndingDate(QDate());
        } else {
            income.setEndingDate(ending_date_->date());
        }
        income.setRecurrence(recurrence_->recurrence());
        if (not index_.isValid()) {
            if (model_->create(income)) {
                emit accept();
            }
        } else {
            if (model_->update(income, index_)) {
                emit accept();
            }
        }
    } else if (button == buttons_->button(QDialogButtonBox::Reset)) {
        if (index_.isValid()) {
            showIncome(index_);
        } else {
            resetForNewIncome(income_.account());
        }
    } else if (button == buttons_->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

} // budget namespace
} // ub namespace
