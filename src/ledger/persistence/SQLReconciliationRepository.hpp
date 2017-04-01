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

    int64_t create(const Reconciliation & reconciliation) override;

    Reconciliation getReconciliation(int64_t id) override;

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
