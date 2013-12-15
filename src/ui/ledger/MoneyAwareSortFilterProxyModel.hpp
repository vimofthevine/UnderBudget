/*
 * Copyright 2013 Kyle Treubig
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

#ifndef MONEYAWARESORTFILTERPROXYMODEL_HPP
#define MONEYAWARESORTFILTERPROXYMODEL_HPP

// Qt include(s)
#include <QSortFilterProxyModel>

namespace ub {

/**
 * A custom QSortFilterProxyModel that is aware of
 * and can properly sort Money values.
 */
class MoneyAwareSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new Money-aware sort/filter proxy model.
	 */
	MoneyAwareSortFilterProxyModel(QObject* parent = 0);

	/**
	 * Reimplemented to properly sort Money values.
	 */
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

}

#endif //MONEYAWARESORTFILTERPROXYMODEL_HPP
