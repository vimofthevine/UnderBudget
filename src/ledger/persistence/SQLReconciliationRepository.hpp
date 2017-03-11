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
#include <memory>
#include <vector>

// Qt include(s)
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Reconciliation.hpp>
#include <ledger/model/ReconciliationRepository.hpp>

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountRepository;

/**
 * Reconciliation repository implementation that uses an SQLite database.
 *
 * @ingroup ledger
 */
class SQLReconciliationRepository : public ReconciliationRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the reconciliation table.
     *
     * @param db       SQL database connection
     * @param accounts Account repository
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLReconciliationRepository(QSqlDatabase & db, std::shared_ptr<AccountRepository> accounts);

    int create(const Reconciliation & reconciliation) override;

    Reconciliation getReconciliation(int id) override;

    std::vector<Reconciliation> getReconciliations(const Account & account) override;

    QString lastError() const override;

    bool remove(const Reconciliation & reconciliation) override;

    bool update(const Reconciliation & reconciliation) override;

private:
    /** Reconciliation table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Account repository */
    std::shared_ptr<AccountRepository> accounts_;

    /**
     * Creates a reconciliation record from the given SQL record.
     *
     * @param record SQL record
     * @return Reconciliation record
     */
    Reconciliation toReconciliation(const QSqlRecord & record);
};

} // ledger namespace
} // ub namespace
