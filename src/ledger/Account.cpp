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

// UnderBudget include(s)
#include "ledger/Account.hpp"

namespace ub {

//------------------------------------------------------------------------------
Account::Account(const QString& name, const QSharedPointer<Account>& parent)
	: accountName(name), parentAccount(parent)
{ }

//------------------------------------------------------------------------------
Account::Account(const Account& orig)
	: accountName(orig.accountName), parentAccount(orig.parentAccount)
{ }

//------------------------------------------------------------------------------
Account& Account::operator=(const Account& other)
{
	accountName = other.accountName;
	parentAccount = other.parentAccount;
	return *this;
}

//------------------------------------------------------------------------------
QSharedPointer<Account> Account::parent() const
{
	return parentAccount;
}

//------------------------------------------------------------------------------
QString Account::name() const
{
	return accountName;
}

//------------------------------------------------------------------------------
QString Account::fullName(const QString& delimiter) const
{
	if (parentAccount.isNull())
	{
		return accountName;
	}
	else
	{
		QString parentName = parentAccount->fullName(delimiter);
		if (parentName != "")
			return parentName + delimiter + accountName;
		else
			return accountName;
	}
}

//------------------------------------------------------------------------------
bool Account::operator==(const Account& that) const
{
	bool eitherParentIsNull = parentAccount.isNull()
		|| that.parentAccount.isNull();
	bool bothParentsAreNull = parentAccount.isNull()
		&& that.parentAccount.isNull();

	return (accountName == that.accountName)
		&& (eitherParentIsNull ? bothParentsAreNull
			: *(parentAccount.data()) == *(that.parentAccount.data()));
}

//------------------------------------------------------------------------------
bool Account::operator!=(const Account& that) const
{
	return ! (*this == that);
}

}
