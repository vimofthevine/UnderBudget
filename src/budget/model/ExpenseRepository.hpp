#pragma once

// Qt include(s)
#include <QString>

// Standard include(s)
#include <vector>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include "Expense.hpp"

namespace ub {
namespace budget {

/**
 * Repository of budgeted expenses.
 *
 * @ingroup budget
 */
class ExpenseRepository {
public:
    // Virtual destructor
    virtual ~ExpenseRepository() {}

    /**
     * Creates a new budgeted expense record.
     *
     * @param expense Budgeted expense to be created
     * @return ID assigned to the newly created budgeted expense
     */
    virtual int create(const Expense & expense) = 0;

    /**
     * Retrieves a budgeted expense from the repository with the given ID.
     *
     * If no expense exists for the given ID, an expense with a -1 ID will be returned.
     *
     * @param id Expense ID to be retrieved
     * @return Budgeted expense for the given ID
     */
    virtual Expense expense(int id) = 0;

    /**
     * Retrieves all budgeted expenses for the given envelope.
     *
     * @param envelope Envelope for which to retrieve budgeted expenses
     * @return List of budgeted expenses for the given envelope
     */
    virtual std::vector<Expense> expenses(const ledger::Envelope & envelope) = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() const = 0;

    /**
     * Removes the given budgeted expense from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param expense Budgeted expense to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Expense & expense) = 0;

    /**
     * Updates the given budgeted expense in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param expense Budgeted expense to be updated
     * @return @c true if successful
     */
    virtual bool update(const Expense & expense) = 0;
};

} // budget namespace
} // ub namespace
