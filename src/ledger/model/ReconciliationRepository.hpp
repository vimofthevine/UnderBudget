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

// Qt include(s)
#include <QString>

// Standard include(s)
#include <vector>

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
    virtual int create(const Reconciliation & reconciliation) = 0;

    /**
     * Retrieves a reconciliation record from the repository with the given ID.
     *
     * If no reconciliation exists for the given ID, a reconciliation with a -1 ID will be returned.
     *
     * @param[in] id Reconciliation ID to be retrieved
     * @return Reconciliation for the given ID
     */
    virtual Reconciliation getReconciliation(int id) = 0;

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
