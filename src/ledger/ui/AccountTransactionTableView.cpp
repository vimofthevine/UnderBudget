// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountTransactionModel.hpp"
#include "AccountTransactionTableView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransactionTableView::AccountTransactionTableView(QWidget * parent) : QTableView(parent) {
    setSelectionBehavior(QTableView::SelectRows);
    setSelectionMode(QTableView::SingleSelection);
}

//--------------------------------------------------------------------------------------------------
void AccountTransactionTableView::setModel(QAbstractItemModel * model) {
    QTableView::setModel(model);

    // Give the payee column the most weight
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(AccountTransactionModel::PAYEE, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(AccountTransactionModel::MEMO, QHeaderView::Stretch);

    setAlternatingRowColors(true);
}

} // ledger namespace
} // ub namespace
