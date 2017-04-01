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

// UnderBudget include(s)
#include <budget/model/BudgetRepository.hpp>
#include <ledger/model/LedgerRepository.hpp>

namespace ub {

/**
 * Collection of repositories for the application.
 */
class Repositories : public budget::BudgetRepository, public ledger::LedgerRepository {
public:
    // Virtual destructor
    virtual ~Repositories() {}

    /**
     * Returns a description of the source of the repositories.
     *
     * @return Repository source location
     */
    virtual QString location() const = 0;
};
} // ub namespace
