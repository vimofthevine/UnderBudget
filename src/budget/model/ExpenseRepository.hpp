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

// Standard include(s)
#include <cstdint>
#include <vector>

// Qt include(s)
#include <QString>

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
    virtual int64_t create(const Expense & expense) = 0;

    /**
     * Retrieves a budgeted expense from the repository with the given ID.
     *
     * If no expense exists for the given ID, an expense with a -1 ID will be returned.
     *
     * @param id Expense ID to be retrieved
     * @return Budgeted expense for the given ID
     */
    virtual Expense expense(int64_t id) = 0;

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
