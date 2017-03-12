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
#include <QTableView>

// Forward declaration(s)
class QSortFilterProxyModel;

namespace ub {
namespace budget {

/**
 * Table view for displaying a list of budget entries.
 *
 * @ingroup budget
 */
class BudgetTableView : public QTableView {
    Q_OBJECT

public:
    /**
     * Initializes the budget entry table view.
     *
     * @param parent Parent widget
     */
    BudgetTableView(QWidget * parent);

    void setModel(QAbstractItemModel * model) override;

signals:
    /**
     * Emitted to indicate a request to create a new item.
     */
    void createItem();

    /**
     * Emitted to indicate a request to modify the specified item.
     *
     * @param index Model index of the item to be modified
     */
    void modifyItem(const QModelIndex & index);

    /**
     * Emitted to indicate a request to duplicate the specified item.
     *
     * @param index Model index of the item to be modified
     */
    void duplicateItem(const QModelIndex & index);

    /**
     * Emitted to indicate a request to delete the specified item.
     *
     * @param index Model index of the item to be deleted
     */
    void deleteItem(const QModelIndex & index);

protected:
    void contextMenuEvent(QContextMenuEvent * event) override;

    bool eventFilter(QObject * object, QEvent * event) override;

private:
    /** Sort/filter proxy model */
    QSortFilterProxyModel * filter_;
};

} // budget namespace
} // ub namespace
