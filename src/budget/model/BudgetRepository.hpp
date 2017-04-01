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

// UnderBudget include(s)
#include "ExpenseRepository.hpp"
#include "IncomeRepository.hpp"

namespace ub {
namespace budget {

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

    /**
     * Returns a pointer to the income repository.
     *
     * @return Income repository
     */
    virtual std::shared_ptr<IncomeRepository> incomes() const = 0;
};

} // budget namespace
} // ub namespace
