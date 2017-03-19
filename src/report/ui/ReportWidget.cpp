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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QtCharts>
#include <QtWidgets>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <budget/model/Expense.hpp>
#include <budget/model/Impact.hpp>
#include <budget/model/Recurrence.hpp>
#include "ImpactModel.hpp"
#include "ReportWidget.hpp"

using namespace QtCharts;

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
ReportWidget::ReportWidget(QWidget * parent)
        : QWidget(parent), content_(new QStackedWidget(this)), beginning_date_(new QDateEdit(this)),
          ending_date_(new QDateEdit(this)), scope_(new QComboBox(this)),
          actual_beginning_balance_(new QLineEdit(this)),
          actual_ending_balance_(new QLineEdit(this)),
          budgeted_ending_balance_(new QLineEdit(this)),
          cash_flow_(new QChart), impacts_(new ImpactModel),
          impact_filter_(new QSortFilterProxyModel) {
    auto today = QDate::currentDate();
    beginning_date_->setCalendarPopup(true);
    beginning_date_->setDate(QDate(today.year(), today.month(), 1));
    ending_date_->setCalendarPopup(true);
    ending_date_->setDate(QDate(today.year(), today.month(), 1).addMonths(1).addDays(-1));

    scope_->addItem(tr("Weekly"), QVariant::fromValue(budget::Recurrence::Weekly));
    scope_->addItem(tr("Monthly"), QVariant::fromValue(budget::Recurrence::Monthly));

    actual_beginning_balance_->setReadOnly(true);
    actual_ending_balance_->setReadOnly(true);
    budgeted_ending_balance_->setReadOnly(true);

    cash_flow_->setTitle(tr("Cash Flow Report"));
    cash_flow_->legend()->setAlignment(Qt::AlignBottom);
    cash_flow_->setAnimationOptions(QChart::SeriesAnimations);
    auto cash_flow =  new QChartView(cash_flow_);
    cash_flow->setRenderHint(QPainter::Antialiasing, true);

    impact_filter_->setSourceModel(impacts_);
    auto projected = new QTableView;
    projected->setModel(impact_filter_);
    projected->sortByColumn(ImpactModel::DATE, Qt::AscendingOrder);
    projected->setSelectionBehavior(QTableView::SelectRows);
    projected->setAlternatingRowColors(true);
    projected->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    projected->horizontalHeader()->setSectionResizeMode(ImpactModel::CATEGORY, QHeaderView::Stretch);
    projected->horizontalHeader()->setSectionResizeMode(ImpactModel::DESCRIPTION, QHeaderView::Stretch);

    content_->addWidget(cash_flow);
    content_->addWidget(projected);

    auto report = new QComboBox;
    connect(report, QOverload<int>::of(&QComboBox::currentIndexChanged), content_,
            &QStackedWidget::setCurrentIndex);
    report->addItem(tr("Cash Flow Report"));
    report->addItem(tr("Projected Expenses and Incomes"));

    auto refresh = new QPushButton(tr("Refresh"));
    connect(refresh, &QPushButton::clicked, this, &ReportWidget::refresh);

    auto params = new QHBoxLayout;
    params->addWidget(new QLabel(tr("Begin")));
    params->addWidget(beginning_date_);
    params->addWidget(new QLabel(tr("End")));
    params->addWidget(ending_date_);
    params->addWidget(scope_);
    params->addStretch();
    params->addWidget(report);
    params->addWidget(refresh);

    auto highlights = new QGridLayout;
    highlights->addWidget(new QLabel(tr("Beginning Balance")), 0, 0);
    highlights->addWidget(actual_beginning_balance_, 0, 1);
    highlights->addWidget(new QLabel(tr("Ending Balance")), 0, 2);
    highlights->addWidget(actual_ending_balance_, 0, 3);
    highlights->addWidget(new QLabel(tr("Budgeted Ending Balance")), 1, 2);
    highlights->addWidget(budgeted_ending_balance_, 1, 3);

    auto layout = new QVBoxLayout;
    layout->addLayout(params);
    layout->addLayout(highlights);
    layout->addWidget(content_);
    setLayout(layout);
}

QDateTimeAxis * dateAxis() {
    auto axis = new QDateTimeAxis;
    axis->setFormat("yyyy-MM-dd");
    return axis;
}

QValueAxis * moneyAxis(const ledger::Currency & currency = ledger::Currency()) {
    auto axis = new QValueAxis;
    axis->setLabelFormat(currency.symbol() + "%.2f");
    return axis;
}

//--------------------------------------------------------------------------------------------------
void ReportWidget::refresh() {
    if (not repos_) {
        emit error(tr("No database has been provided"));
    }

    QDate start = beginning_date_->date();
    QDate stop = ending_date_->date();
    if (stop <= start) {
        emit error(tr("Invalid dates selected, ending date must be later than beginning date"));
    }

    budget_.reset(start, stop);

    auto expense_repo = repos_->expenses();
    auto expenses = expense_repo->expenses(start, stop);
    for (auto & expense : expenses) {
        budget_.add(expense);
    }

    auto income_repo = repos_->incomes();
    auto incomes = income_repo->incomes(start, stop);
    for (auto & income : incomes) {
        budget_.add(income);
    }

    budget::Recurrence recurrence;
    recurrence.setPeriodicity(1);
    recurrence.setScope(scope_->currentData().value<budget::Recurrence::ScopeType>());
    if (recurrence.scope() == budget::Recurrence::Weekly) {
        recurrence.setDay(start.dayOfWeek());
    }

    compiler_.compile(start, stop, recurrence, budget_.impacts());
    impacts_->setImpacts(budget_.impacts());

    actual_beginning_balance_->setText(compiler_.beginningBalance().toString());
    actual_ending_balance_->setText(compiler_.endingBalance().toString());
    budgeted_ending_balance_->setText(
                (compiler_.beginningBalance() + budget_.netChange()).toString());

    populateCashFlowChart();
}

//--------------------------------------------------------------------------------------------------
void ReportWidget::populateCashFlowChart() {
    for (auto & series : cash_flow_->series()) {
        cash_flow_->removeSeries(series);
    }

    auto budgeted = new QLineSeries;
    budgeted->setName(tr("Budgeted"));
    compiler_.budgetedBalances(budgeted);
    cash_flow_->addSeries(budgeted);

    auto historical = new QLineSeries;
    historical->setName(tr("Actual"));
    compiler_.historicalBalances(historical);
    cash_flow_->addSeries(historical);

    // Create the default axes to get the chart scaled correctly, then remove them
    cash_flow_->createDefaultAxes();
    for (auto & axis : cash_flow_->axes()) {
        cash_flow_->removeAxis(axis);
    }

    // Then add the customized axes
    auto x_axis = dateAxis();
    auto y_axis = moneyAxis();
    cash_flow_->addAxis(x_axis, Qt::AlignBottom);
    cash_flow_->addAxis(y_axis, Qt::AlignLeft);

    historical->attachAxis(x_axis);
    historical->attachAxis(y_axis);
    budgeted->attachAxis(x_axis);
    budgeted->attachAxis(y_axis);
}

//--------------------------------------------------------------------------------------------------
void ReportWidget::setRepository(std::shared_ptr<Repositories> repositories) {
    repos_ = repositories;
    compiler_.setRepository(repos_->transactions());
    refresh();
}

} // report namespace
} // ub namespace
