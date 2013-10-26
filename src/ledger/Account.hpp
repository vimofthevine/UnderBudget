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

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QString>

namespace ub {

/**
 * An account represents a bucket of money, from which funds are taken or into
 * which funds are transferred in a transaction. A single transaction always
 * has exactly two accounts--the withdrawal account (from which funds are taken)
 * and the deposit account (to which funds are added).
 *
 * Common account types include:
 * - asset accounts, such as bank accounts, cash, or credit cards
 * - income accounts, such as a salary or paycheck
 * - expense accounts, such as food, utilities, or entertainment
 *
 * Depending on the source of the ledger data, accounts may be organized in a
 * hierarchical manner. For example, accounts imported from GnuCash are
 * structured by their type (e.g., Income:Salary, Asset:Checking Account,
 * Expense:Entertainment:Movies), while those imported from a banking or credit
 * card statement dump (e.g., CSV) may only list the deposit account (assuming
 * the withdrawal account is the bank or credit card itself).
 *
 * @ingroup ledger
 */
class Account
{
public:
	/**
	 * Constructs an account with the given name and parent account.
	 *
	 * @param[in] name   account name
	 * @param[in] parent parent account, if in a hierarchy structure
	 */
	explicit Account(const QString& name = "",
	                 const QSharedPointer<Account>& parent = QSharedPointer<Account>());

	/**
	 * Constructs an account as a copy of the given account.
	 *
	 * @param[in] orig account to be copied
	 */
	Account(const Account& orig);

	/**
	 * Assigns the given account to this account.
	 *
	 * @param[in] other new account value
	 */
	Account& operator=(const Account& other);

	/**
	 * Returns the parent of this account.
	 *
	 * @return parent account
	 */
	QSharedPointer<Account> parent() const;

	/**
	 * Returns the name of this account.
	 *
	 * @return account name
	 */
	QString name() const;

	/**
	 * Returns the full name of this account. The full name is the names of
	 * this account and all parents of this account, combined with the specified
	 * name delimiter.
	 *
	 * For example, an accounts named "parent", "child", and "grandchild" and
	 * structured likewise, the full name of the grandchild would be
	 * "parent:child:grandchild".
	 *
	 * If the highest-level account's name is empty, it is left off of the
	 * full name. For example, if parent's name had been empty (""), then the
	 * full name of grandchild would be "child:grandchild". This is done for
	 * cases where a root account exists but does not contribute to account
	 * full names.
	 *
	 * @param[in] delimiter character string to inject between
	 * @return account's full name, which is a combination of the names of
	 *         this account and all parents of this account
	 */
	QString fullName(const QString& delimiter = ":") const;

	/**
	 * Checks if the given account is equal to this account. Two accounts
	 * are equal if their names are identical and their parent accounts are
	 * the equal.
	 *
	 * @param[in] that account to be compared against this account
	 * @return `true` if the given account is equal to this account
	 */
	bool operator==(const Account& that) const;

	/**
	 * Checks if the given account is not equal to this account. Two accounts
	 * are not equal if either their names are different or if their parent
	 * accounts are not equal.
	 *
	 * @param[in] that account to be compared against this account
	 * @return `true` if the given account is not equal to this account
	 */
	bool operator!=(const Account& that) const;

private:
	/** Pointer to the parent of this account  */
	QSharedPointer<Account> parentAccount;

	/** Name of this account */
	QString accountName;
};

}

#endif //ACCOUNT_HPP
