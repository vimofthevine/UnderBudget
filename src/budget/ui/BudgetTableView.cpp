// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "BudgetTableView.hpp"
#include "ExpenseModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
BudgetTableView::BudgetTableView(QWidget * parent)
        : QTableView(parent), filter_(new QSortFilterProxyModel(this)) {
    QTableView::setModel(filter_);
    setSortingEnabled(true);
    sortByColumn(ExpenseModel::BEGINNING_DATE, Qt::AscendingOrder);

    setSelectionBehavior(QTableView::SelectRows);
    setSelectionMode(QTableView::SingleSelection);

    setAlternatingRowColors(true);

    connect(this, &QTableView::doubleClicked, this, [this](const QModelIndex & index) {
        emit modifyItem(this->filter_->mapToSource(index));
    });

    installEventFilter(this);
}

//--------------------------------------------------------------------------------------------------
void BudgetTableView::setModel(QAbstractItemModel * model) {
    filter_->setSourceModel(model);

    // Give the description and recurrence columns the most weight
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(ExpenseModel::DESCRIPTION, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(ExpenseModel::RECURRENCE, QHeaderView::Stretch);
}

//--------------------------------------------------------------------------------------------------
void BudgetTableView::contextMenuEvent(QContextMenuEvent * event) {
    auto index = indexAt(event->pos());
    if (index.isValid()) {
        auto mod = new QAction(tr("Edit"), this);
        connect(mod, &QAction::triggered, this, [this]() {
            if (currentIndex().isValid()) {
                emit modifyItem(filter_->mapToSource(currentIndex()));
            }
        });

        auto dup = new QAction(tr("Duplicate"), this);
        connect(dup, &QAction::triggered, this, [this]() {
            if (currentIndex().isValid()) {
                emit duplicateItem(filter_->mapToSource(currentIndex()));
            }
        });

        auto del = new QAction(tr("Delete"), this);
        connect(del, &QAction::triggered, this, [this]() {
            if (currentIndex().isValid()) {
                emit deleteItem(filter_->mapToSource(currentIndex()));
            }
        });

        auto menu = new QMenu(this);
        menu->addAction(mod);
        menu->addAction(dup);
        menu->addAction(del);
        menu->exec(event->globalPos());
    }
}

//--------------------------------------------------------------------------------------------------
bool BudgetTableView::eventFilter(QObject * object, QEvent * event) {
    if (currentIndex().isValid()) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent * key_event = static_cast<QKeyEvent *>(event);

            if ((key_event->key() == Qt::Key_Enter) or (key_event->key() == Qt::Key_Return)) {
                emit modifyItem(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_D) {
                emit duplicateItem(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_Delete) {
                emit deleteItem(filter_->mapToSource(currentIndex()));
            }
        }
    }

    return QTableView::eventFilter(object, event);
}

} // budget namespace
} // ub namespace