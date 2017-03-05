#pragma once

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>

namespace ub {
namespace budget {

/**
 * Expense repository implementation that uses an SQLite database.
 *
 * @ingroup budget
 */
class SQLExpenseRepository : public ExpenseRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the budget_expense table.
     *
     * @param db        SQL database connection
     * @param envelopes Envelope repository
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLExpenseRepository(QSqlDatabase & db, std::shared_ptr<ledger::EnvelopeRepository> envelopes);

    int create(const Expense & expense) override;

    Expense expense(int id) override;

    std::vector<Expense> expenses(const ledger::Envelope & account) override;

    QString lastError() const override;

    bool remove(const Expense & expense) override;

    bool update(const Expense & expense) override;

private:
    /** Budgeted expense table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Envelope repository */
    std::shared_ptr<ledger::EnvelopeRepository> envelopes_;

    /**
     * Creates a budgeted expense from the given SQL record.
     *
     * @param record SQL record
     * @return Budgeted expense
     */
    Expense toExpense(const QSqlRecord & record);
};

} // budget namespace
} // ub namespace
