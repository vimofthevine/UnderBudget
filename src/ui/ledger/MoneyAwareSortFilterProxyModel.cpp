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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "ui/ledger/MoneyAwareSortFilterProxyModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
MoneyAwareSortFilterProxyModel::MoneyAwareSortFilterProxyModel(QObject* parent)
	: QSortFilterProxyModel(parent)
{ }

//------------------------------------------------------------------------------
bool MoneyAwareSortFilterProxyModel::lessThan(const QModelIndex& left,
	const QModelIndex& right) const
{
	QVariant leftData = sourceModel()->data(left, Qt::EditRole);
	if (leftData.canConvert<Money>())
	{
		QVariant rightData = sourceModel()->data(right, Qt::EditRole);
		Money leftMoney = leftData.value<Money>();
		Money rightMoney = rightData.value<Money>();
		return (leftMoney < rightMoney);
	}
	else
		return QSortFilterProxyModel::lessThan(left, right);
}

}

