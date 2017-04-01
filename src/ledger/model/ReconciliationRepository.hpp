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
#include <cstdint>
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Account.hpp"
#include "Reconciliation.hpp"

namespace ub {
namespace ledger {

/**
 * Repository of reconciliation records.
 *
 * @ingroup ledger
 */
class ReconciliationRepository {
public:
    // Virtual destructor
    virtual ~ReconciliationRepository() {}

    /**
     * Creates a new reconciliation record.
     *
     * @param reconciliation Reconciliation record to be created
     * @return ID assigned to the newly created reconciliation record
     */
    virtual int64_t create(const Reconciliation & reconciliation) = 0;

    /**
     * Retrieves a reconciliation record from the repository with the given ID.
     *
     * If no reconciliation exists for the given ID, a reconciliation with a -1 ID will be returned.
     *
     * @param[in] id Reconciliation ID to be retrieved
     * @return Reconciliation for the given ID
     */
    virtual Reconciliation getReconciliation(int64_t id) = 0;

    /**
     * Retrieves all reconciliation records for the given account.
     *
     * @param account Account for which to retrieve reconciliation records
     * @return List of reconciliation records for the given account
     */
    virtual std::vector<Reconciliation> getReconciliations(const Account & account) = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() const = 0;

    /**
     * Removes the given reconciliation record from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param reconciliation Reconciliation record to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Reconciliation & reconciliation) = 0;

    /**
     * Updates the given reconciliation record in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param reconciliation Reconciliation record to be updated
     * @return @c true if successful
     */
    virtual bool update(const Reconciliation & reconciliation) = 0;
};

} // ledger namespace
} // ub namespace
