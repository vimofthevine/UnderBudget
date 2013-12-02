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
#include <QtCore>

// UnderBudget include(s)
#include "ledger/Account.hpp"
#include "ledger/ImportedTransaction.hpp"

namespace ub {

//------------------------------------------------------------------------------
ImportedTransaction::ImportedTransaction(uint id, const QDate& date,
		const Money& amount, const QString& payee, const QString& memo,
		const QSharedPointer<Account>& withdrawal,
		const QSharedPointer<Account>& deposit)
	: id(id), postedDate(date), transferredAmount(amount),
	  payeeDesc(payee), memoDesc(memo),
	  withdrawal(withdrawal), deposit(deposit)
{ }

//------------------------------------------------------------------------------
uint ImportedTransaction::transactionId() const
{
	return id;
}

//------------------------------------------------------------------------------
QDate ImportedTransaction::date() const
{
	return postedDate;
}

//------------------------------------------------------------------------------
Money ImportedTransaction::amount() const
{
	return transferredAmount;
}

//------------------------------------------------------------------------------
QString ImportedTransaction::payee() const
{
	return payeeDesc;
}

//------------------------------------------------------------------------------
QString ImportedTransaction::memo() const
{
	return memoDesc;
}

//------------------------------------------------------------------------------
QString ImportedTransaction::withdrawalAccount() const
{
	return withdrawal->fullName();
}

//------------------------------------------------------------------------------
QString ImportedTransaction::depositAccount() const
{
	return deposit->fullName();
}

//------------------------------------------------------------------------------
bool operator<(const ImportedTransaction& lhs, const ImportedTransaction& rhs)
{
	if (lhs.postedDate != rhs.postedDate)
		return (lhs.postedDate < rhs.postedDate);
	if (lhs.payeeDesc != rhs.payeeDesc)
		return (lhs.payeeDesc < rhs.payeeDesc);
	if (lhs.memoDesc != rhs.memoDesc)
		return (lhs.memoDesc < rhs.memoDesc);
	if (lhs.depositAccount() != rhs.depositAccount())
		return (lhs.depositAccount() < rhs.depositAccount());
	else
		return (lhs.transferredAmount < rhs.transferredAmount);
}


}

