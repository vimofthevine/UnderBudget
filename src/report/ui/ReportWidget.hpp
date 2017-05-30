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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QString>
#include <QWidget>

// UnderBudget include(s)
#include <budget/model/Budget.hpp>
#include <report/model/Compiler.hpp>

// Forward declaration(s)
class QDateEdit;
class QComboBox;
class QLineEdit;
class QSortFilterProxyModel;
class QStackedWidget;

namespace QtCharts {
// Forward declaration(s)
class QChart;
}

namespace ub {

// Forward declaration(s)
class Repositories;

namespace ledger {
// Forward declaration(s)
class Account;
class Envelope;
}

namespace report {

// Forward declaration(s)
class ImpactModel;
class ProjectedExpenseModel;
class ProjectedIncomeModel;

/**
 * Widget for displaying reports.
 *
 * @ingroup report
 */
class ReportWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * Initializes the report widget.
     *
     * @param parent Parent widget
     */
    ReportWidget(QWidget * parent);

    /**
     * Updates the repositories used by the view.
     *
     * @param repositories Content repositories
     */
    void setRepository(std::shared_ptr<Repositories> repositories);

signals:
    /** Emitted when an error has occurred. */
    void error(const QString & message) const;

    /**
     * Emitted to display the incomes for the given account.
     *
     * @param account Account to be shown
     */
    void showAccountIncomes(const ledger::Account & account);

    /**
     * Emitted to display the expenses for the given envelope.
     *
     * @param envelope Envelope to be shown
     */
    void showEnvelopeExpenses(const ledger::Envelope & envelope);

private slots:
    /**
     * Refreshes the displayed reports.
     */
    void refresh();

private:
    /** Report compiler */
    Compiler compiler_;
    /** Budget */
    budget::Budget budget_;

    /** Content repositories */
    std::shared_ptr<Repositories> repos_;

    /** Stacked content widget */
    QStackedWidget * content_;
    /** Beginning date selection widget */
    QDateEdit * beginning_date_;
    /** Ending date selection widget */
    QDateEdit * ending_date_;
    /** Scope selection widget */
    QComboBox * scope_;

    /** Historical beginning balance */
    QLineEdit * actual_beginning_balance_;
    /** Historical Ending balance */
    QLineEdit * actual_ending_balance_;
    /** Budgeted Ending balance */
    QLineEdit * budgeted_ending_balance_;

    /** Cash flow chart */
    QtCharts::QChart * cash_flow_;

    /** Projected expenses model */
    ProjectedExpenseModel * projected_expenses_;
    /** Sorted projected expenses model */
    QSortFilterProxyModel * projected_expenses_filter_;
    /** Projected incomes model */
    ProjectedIncomeModel * projected_incomes_;
    /** Expanded budgeted impacts model */
    ImpactModel * expanded_impacts_;
    /** Expanded budgeted impact filter model */
    QSortFilterProxyModel * expanded_impact_filter_;

    /**
     * Populates the cash flow chart report.
     */
    void populateCashFlowChart();
};

} // report namespace
} // ub namespace
