#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QAbstractItemModel>
#include <QList>
#include <QModelIndex>
#include <QVariant>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountRepository;
class TransactionRepository;

/**
 * Hierarchical account view model.
 *
 * @ingroup ledger
 */
class AccountModel : public QAbstractItemModel {
    Q_OBJECT

public:
    /** Account name column */
    static const int NAME = 0;
    /** Account balance column */
    static const int BALANCE = 1;

    /**
     * Initializes the account view model.
     *
     * @param[in] accounts     Account repository
     * @param[in] transactions Transaction repository
     */
    AccountModel(std::shared_ptr<AccountRepository> accounts,
                 std::shared_ptr<TransactionRepository> transactions);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

private:
    /** Account repository */
    std::shared_ptr<AccountRepository> accounts_;
    /** Transaction repository */
    std::shared_ptr<TransactionRepository> transactions_;
    /** Header titles */
    QList<QString> headers_;
};

} // ledger namespace
} // ub namespace
