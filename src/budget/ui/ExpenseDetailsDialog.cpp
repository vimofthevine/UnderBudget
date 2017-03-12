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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/ui/DoubleLineEdit.hpp>
#include "ExpenseModel.hpp"
#include "ExpenseDetailsDialog.hpp"
#include "RecurrenceEditWidget.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
ExpenseDetailsDialog::ExpenseDetailsDialog(ExpenseModel * model, QWidget * parent)
        : QDialog(parent), model_(model), description_(new QLineEdit(this)),
          beginning_date_(new QDateEdit(this)), ending_date_(new QDateEdit(this)),
          recurrence_(new RecurrenceEditWidget(this)), amount_(new ledger::DoubleLineEdit(this)),
          buttons_(new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset |
                                        QDialogButtonBox::Cancel)) {
    description_->setPlaceholderText(tr("Expense description"));
    beginning_date_->setCalendarPopup(true);
    ending_date_->setCalendarPopup(true);
    ending_date_->setSpecialValueText(tr("Never"));

    connect(buttons_, &QDialogButtonBox::clicked, this, &ExpenseDetailsDialog::clicked);

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

    setWindowTitle(tr("Budgeted Expense"));
}

//--------------------------------------------------------------------------------------------------
void ExpenseDetailsDialog::resetForNewExpense(const ledger::Envelope & envelope) {
    index_ = QModelIndex();
    expense_ = Expense();
    expense_.setEnvelope(envelope);
    description_->clear();
    beginning_date_->setDate(QDate::currentDate());
    ending_date_->setDate(ending_date_->minimumDate());
    recurrence_->clear();
    amount_->clear();
    show();
}

//--------------------------------------------------------------------------------------------------
void ExpenseDetailsDialog::showExpense(const QModelIndex & index) {
    index_ = index;
    expense_ = model_->expense(index);
    description_->setText(expense_.description());
    beginning_date_->setDate(expense_.beginningDate());
    if (expense_.endingDate().isValid()) {
        ending_date_->setDate(expense_.endingDate());
    } else {
        ending_date_->setDate(ending_date_->minimumDate());
    }
    recurrence_->setRecurrence(expense_.recurrence());
    amount_->setValue(expense_.amount().amount());
    show();
}

//--------------------------------------------------------------------------------------------------
void ExpenseDetailsDialog::duplicateExpense(const QModelIndex & index) {
    showExpense(index);
    index_ = QModelIndex();
}

//--------------------------------------------------------------------------------------------------
void ExpenseDetailsDialog::clicked(QAbstractButton * button) {
    if (button == buttons_->button(QDialogButtonBox::Save)) {
        Expense expense = expense_;
        expense.setAmount(ledger::Money(amount_->value(), expense.envelope().currency()));
        expense.setBeginningDate(beginning_date_->date());
        expense.setDescription(description_->text());
        if (ending_date_->date() == ending_date_->minimumDate()) {
            expense.setEndingDate(QDate());
        } else {
            expense.setEndingDate(ending_date_->date());
        }
        expense.setRecurrence(recurrence_->recurrence());
        if (not index_.isValid()) {
            if (model_->create(expense)) {
                emit accept();
            }
        } else {
            if (model_->update(expense, index_)) {
                emit accept();
            }
        }
    } else if (button == buttons_->button(QDialogButtonBox::Reset)) {
        if (index_.isValid()) {
            showExpense(index_);
        } else {
            resetForNewExpense(expense_.envelope());
        }
    } else if (button == buttons_->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

} // budget namespace
} // ub namespace
