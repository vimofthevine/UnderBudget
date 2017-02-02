// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountListWidget.hpp"
#include "AccountModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountListWidget::AccountListWidget(AccountModel *model, QWidget *parent)
        : QTreeView(parent), model_(model) {
    setModel(model_);
    expandAll();

    // Give the name column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
}

} // ledger namespace
} // ub namespace
