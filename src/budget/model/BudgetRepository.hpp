#pragma once

// Standard include(s)
#include <memory>

namespace ub {
namespace budget {

// Forward declaration(s)
class ExpenseRepository;

/**
 * Repository of budget repositories.
 */
class BudgetRepository {
public:
    // Virtual destructor
    virtual ~BudgetRepository() {}

    /**
     * Returns a pointer to the expense repository.
     *
     * @return Expense repository
     */
    virtual std::shared_ptr<ExpenseRepository> expenses() const = 0;
};

} // budget namespace
} // ub namespace
