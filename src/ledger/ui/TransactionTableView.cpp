// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountTransactionModel.hpp"
#include "TransactionTableView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
TransactionTableView::TransactionTableView(QWidget * parent)
        : QTableView(parent), filter_(new QSortFilterProxyModel(this)) {
    QTableView::setModel(filter_);
    setSortingEnabled(true);
    sortByColumn(AccountTransactionModel::DATE, Qt::AscendingOrder);

    setSelectionBehavior(QTableView::SelectRows);
    setSelectionMode(QTableView::SingleSelection);

    setAlternatingRowColors(true);
}

//--------------------------------------------------------------------------------------------------
void TransactionTableView::setModel(QAbstractItemModel * model) {
    filter_->setSourceModel(model);

    // Give the payee and memo columns the most weight
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(AccountTransactionModel::PAYEE, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(AccountTransactionModel::MEMO, QHeaderView::Stretch);
}

} // ledger namespace
} // ub namespace