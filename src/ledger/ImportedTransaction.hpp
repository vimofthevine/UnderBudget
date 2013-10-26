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

#ifndef IMPORTEDTRANSACTION_HPP
#define IMPORTEDTRANSACTION_HPP

// Qt include(s)
#include <QDate>
#include <QSharedPointer>
#include <QString>

// UnderBudget include(s)
#include "accounting/Money.hpp"

namespace ub {

// Forward declaration(s)
class Account;

/**
 * An immutable representation of a transfer of funds.
 */
class ImportedTransaction
{
public:
	/**
	 * Constructs a new imported transaction.
	 *
	 * @param[in] id         transaction ID
	 * @param[in] date       transaction date
	 * @param[in] amount     transferred amount
	 * @param[in] payee      payee
	 * @param[in] memo       memo
	 * @param[in] withdrawal withdrawal account
	 * @param[in] deposit    deposit account
	 */
	ImportedTransaction(uint id, const QDate& date, const Money& amount,
		const QString& payee, const QString& memo,
		const QSharedPointer<Account>& withdrawal,
		const QSharedPointer<Account>& deposit);

	/**
	 * Returns the unique ID of this transaction. The transaction ID
	 * allows for querying of assignments.
	 *
	 * @return unique ID of this transaction
	 */
	uint transactionId() const;

	/**
	 * Returns the date on which the transaction occurred.
	 *
	 * @return date on which the transaction occurred
	 */
	QDate date() const;

	/**
	 * Returns the amount of money that has been transferred.
	 *
	 * @return amount of money that has been transferred
	 */
	Money amount() const;

	/**
	 * Returns the payee description.
	 *
	 * @return transaction payee
	 */
	QString payee() const;

	/**
	 * Returns the transaction memo.
	 *
	 * @return transaction memo
	 */
	QString memo() const;

	/**
	 * Returns the account from which funds were taken.
	 *
	 * @return withdrawal account
	 */
	QString withdrawalAccount() const;

	/**
	 * Returns the account to which funds were added.
	 *
	 * @return deposit account
	 */
	QString depositAccount() const;

private:
	/** Transaction ID */
	const uint id;
	/** Transaction date */
	const QDate postedDate;
	/** Transferred amount */
	const Money transferredAmount;
	/** Payee */
	const QString payeeDesc;
	/** Memo */
	const QString memoDesc;
	/** Withdrawal account */
	const QSharedPointer<Account> withdrawal;
	/** Deposit account */
	const QSharedPointer<Account> deposit;
};

}

#endif //IMPORTEDTRANSACTION_HPP
