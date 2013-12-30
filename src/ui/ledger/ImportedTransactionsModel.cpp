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
#include "analysis/Assignments.hpp"
#include "budget/Estimate.hpp"
#include "ui/ledger/ImportedTransactionsModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int NUM_COLS = 11;
const int TRN_ID_COL = 0;
const int EST_ID_COL = 1;
const int RULE_ID_COL = 2;
const int DATE_COL = 3;
const int PAYEE_COL = 4;
const int MEMO_COL = 5;
const int AMOUNT_COL = 6;
const int WITHDRAWAL_COL = 7;
const int DEPOSIT_COL = 8;
const int ASSIGNED_COL = 9;
const int ESTIMATE_COL = 10;

//------------------------------------------------------------------------------
ImportedTransactionsModel::ImportedTransactionsModel(QSharedPointer<Estimate> estimates,
		Assignments* assignments, QObject* parent)
	: QAbstractTableModel(parent), estimates(estimates), assignments(assignments)
{ }

//------------------------------------------------------------------------------
int ImportedTransactionsModel::columnCount(const QModelIndex& parent) const
{
	return NUM_COLS;
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::headerData(int section,
	Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case TRN_ID_COL:
			return tr("Transaction ID");
		case EST_ID_COL:
			return tr("Estimate ID");
		case RULE_ID_COL:
			return tr("Assignment Rule ID");
		case DATE_COL:
			return tr("Date");
		case PAYEE_COL:
			return tr("Payee");
		case MEMO_COL:
			return tr("Memo");
		case AMOUNT_COL:
			return tr("Amount");
		case WITHDRAWAL_COL:
			return tr("Withdrawal Account");
		case DEPOSIT_COL:
			return tr("Deposit Account");
		case ASSIGNED_COL:
			return tr("Assigned?");
		case ESTIMATE_COL:
			return tr("Estimate");
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

//------------------------------------------------------------------------------
int ImportedTransactionsModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;
	return transactions.size();
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();
	if (role == Qt::CheckStateRole)
		return checkStateData(index);
	if (role == Qt::DisplayRole)
		return displayData(index);
	if (role == Qt::EditRole)
		return editData(index);
	if (role == Qt::ToolTipRole)
		return toolTipData(index);
	return QVariant();
}

//------------------------------------------------------------------------------
Qt::ItemFlags ImportedTransactionsModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);

	// If the assigned column
	if (index.column() == ASSIGNED_COL)
	{
		flags |= Qt::ItemIsUserCheckable;
	}

	return flags;
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::checkStateData(const QModelIndex& index) const
{
	// Only assigned column is checkable
	if (index.column() != ASSIGNED_COL)
		return QVariant();

	int row = index.row();
	if (row < 0 || row >= transactions.size())
		return QVariant();

	ImportedTransaction transaction = transactions.at(row);
	return (assignments->estimate(transaction.transactionId()) != 0)
		? Qt::Checked : Qt::Unchecked;
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::displayData(const QModelIndex& index) const
{
	int row = index.row();
	if (row < 0 || row >= transactions.size())
		return QVariant();

	ImportedTransaction transaction = transactions.at(row);
	uint id = transaction.transactionId();
	switch (index.column())
	{
	case TRN_ID_COL:
		return id;
	case EST_ID_COL:
		return assignments->estimate(id);
	case RULE_ID_COL:
		return assignments->rule(id);
	case DATE_COL:
		return transaction.date();
	case PAYEE_COL:
		return transaction.payee();
	case MEMO_COL:
		return transaction.memo();
	case AMOUNT_COL:
		return transaction.amount().toString();
	case WITHDRAWAL_COL:
		return transaction.withdrawalAccount();
	case DEPOSIT_COL:
		return transaction.depositAccount();
	case ESTIMATE_COL:
	{
		uint eid = assignments->estimate(id);

		// If transaction was assigned
		if (eid != 0)
		{
			Estimate* estimate = estimates->find(eid);
			if (estimate)
			{
				return estimate->estimateName();
			}
		}

		return QVariant();
	}
	default:
		return QVariant();
	}
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::editData(const QModelIndex& index) const
{
	int row = index.row();
	if (row < 0 || row >= transactions.size())
		return QVariant();

	ImportedTransaction transaction = transactions.at(row);
	switch (index.column())
	{
	case AMOUNT_COL:
	{
		return QVariant::fromValue(transaction.amount());
	}
	default:
		return QVariant();
	}
}

//------------------------------------------------------------------------------
QVariant ImportedTransactionsModel::toolTipData(const QModelIndex& index) const
{
	int row = index.row();
	if (row < 0 || row >= transactions.size())
		return QVariant();

	ImportedTransaction transaction = transactions.at(row);
	// TODO get from assignments model
	return "Transaction assignment info";
}

//------------------------------------------------------------------------------
void ImportedTransactionsModel::setTransactions(const QList<ImportedTransaction>& trns)
{
	beginRemoveRows(QModelIndex(), 0, rowCount()-1);
	transactions.clear();
	endRemoveRows();
	beginInsertRows(QModelIndex(), 0, trns.size()-1);
	transactions = trns;
	endInsertRows();
}

}

