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
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include "Income.hpp"

namespace ub {
namespace budget {

/**
 * Repository of budgeted incomes.
 *
 * @ingroup budget
 */
class IncomeRepository {
public:
    // Virtual destructor
    virtual ~IncomeRepository() {}

    /**
     * Creates a new budgeted income record.
     *
     * @param income Budgeted income to be created
     * @return ID assigned to the newly created budgeted income
     */
    virtual int64_t create(const Income & income) = 0;

    /**
     * Retrieves a budgeted income from the repository with the given ID.
     *
     * If no income exists for the given ID, an income with a -1 ID will be returned.
     *
     * @param id Income ID to be retrieved
     * @return Budgeted income for the given ID
     */
    virtual Income income(int64_t id) = 0;

    /**
     * Retrieves all budgeted incomes for the given account.
     *
     * @param account Account for which to retrieve budgeted incomes
     * @return List of budgeted incomes for the given account
     */
    virtual std::vector<Income> incomes(const ledger::Account & account) = 0;

    /**
     * Retrieves all budgeted incomes valid for the given date range.
     *
     * @param start Beginning date
     * @param stop  Ending date
     * @return List of budgeted incomes for the given date range
     */
    virtual std::vector<Income> incomes(const QDate & start, const QDate & stop) = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() const = 0;

    /**
     * Removes the given budgeted income from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param income Budgeted income to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Income & income) = 0;

    /**
     * Updates the given budgeted income in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param income Budgeted income to be updated
     * @return @c true if successful
     */
    virtual bool update(const Income & income) = 0;
};

} // budget namespace
} // ub namespace
