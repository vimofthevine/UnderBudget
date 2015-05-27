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

#ifndef GNUCASHREADER_HPP
#define GNUCASHREADER_HPP

// Qt include(s)
#include <QDate>
#include <QHash>
#include <QList>
#include <QObject>
#include <QUuid>
#include <QXmlStreamReader>

// UnderBudget include(s)
#include "ledger/storage/ImportedTransactionSource.hpp"

// Forward declaration(s)
class QIODevice;

namespace ub {

/**
 * GnuCash file reader.
 *
 * @ingroup ledger_storage
 */
class GnuCashReader : public QObject
{
	Q_OBJECT

public:
	/**
	 * Creates a new reader not associated with any particular file.
	 */
	GnuCashReader();

	/**
	 * Creates a new reader for the given file.
	 *
	 * @param[in] fileName GnuCash file location
	 */
	GnuCashReader(const QString& fileName);

	/**
	 * Parses the XML from the given IO device. If successful, this will emit
	 * the `finished` signal with a result of `Complete`, and the `imported`
	 * signal will be emitted with the imported transactions. If an error
	 * occured or the import was cancelled, the `finished` signal will be emitted
	 * with the `Cancelled` or `FailedWithError` results.
	 *
	 * If the start and end dates are not valid dates, then all transactions
	 * will be imported. Otherwise only transactions that have occurred
	 * between the start and end dates will be imported.
	 *
	 * For use of this reading in a multi-threaded environment, it is recommended
	 * that the `import` slot be used instead, relying on the `started`, `finished`,
	 * `progress`, and `imported` signals for monitoring progress and retrieving
	 * results. This public method exists for callers that execute in the same
	 * thread.
	 *
	 * @param[in]  device IO device from which to parse as XML
	 * @param[out] trns   imported transaction list
	 * @param[in]  start  start date of import range
	 * @param[in]  end    end date of import range
	 * @return `true` if successful in parsing the XML stream
	 */
	bool read(QIODevice* device, QList<ImportedTransaction>& trns,
		const QDate& start = QDate(), const QDate& end = QDate());

	/**
	 * Returns a description of the last error to occur while reading
	 * the transactions from the GnuCash file.
	 *
	 * @return last error
	 */
	QString errorString() const;

public slots:
	/**
	 * Reads the given file and imports all transactions that occurred
	 * between the given start and end dates.
	 *
	 * If the start and end dates are not valid dates, then all transactions
	 * will be imported.
	 *
	 * @param[in] start    start date of import range
	 * @param[in] end      end date of import range
	 */
	void import(const QDate& start = QDate(), const QDate& end = QDate());

	/**
	 * Cancels the current import operation, if one is in progress.
	 */
	void cancel();

signals:
	/**
	 * Emitted when an import operation commences.
	 */
	void started();

	/**
	 * Emitted when an import operation is completed.
	 *
	 * When a failure has occurred, `message` will contain the error
	 * message as detected. For a successful import, `message` will
	 * indicate the number of transactions that were imported.
	 *
	 * @param result  import result
	 * @param message import message
	 */
	void finished(ImportedTransactionSource::Result result,
		const QString& message);

	/**
	 * If emitted, indicates the current progress of the
	 * transaction import as a percentage (out of 100).
	 *
	 * @param percent import percent complete
	 */
	void progress(int percent);

	/**
	 * Emitted when the import has finished and has successfully imported
	 * a list of transactions. If the import failed or was cancelled, this
	 * signal is not emitted.
	 *
	 * @param transactions imported transactions
	 */
	void imported(QList<ImportedTransaction> transactions);

private:
	/** Location of the GnuCash file */
	const QString fileName;

	/** XML stream reader */
	QXmlStreamReader xml;

	/** Imported accounts */
	QHash<QUuid, QSharedPointer<Account> > accounts;

	/** Start date of import range filter */
	QDate startDateFilter;

	/** End date of import range filter */
	QDate endDateFilter;

	/**
	 * GnuCash account type enumeration.
	 */
	enum AccountType
	{
		UnknownAccountType,
		Asset,
		Bank,
		Cash,
		Credit,
		Equity,
		Expense,
		Income,
		Liability,
		Root
	};

	/** Account types */
	QHash<QUuid, AccountType> accountTypes;

	/** Imported transactions */
	QList<ImportedTransaction> transactions;

	/**
	 * GnuCash transaction split.
	 */
	struct TransactionSplit
	{
		/** Split UUID */
		QUuid uid;
		/** Split memo */
		QString memo;
		/** Split value */
		double value;
		/** Split account */
		QUuid account;
	};

	/**
	 * Parses the current XML stream as a GnuCash file.
	 */
	void readGncFile();

	/**
	 * Parses the current XML stream as a GnuCash version 2.0.0 book.
	 */
	void readVersion2Book();

	/**
	 * Parses the current XML stream for a version 2.0.0 account definition.
	 *
	 * An error may be raised for the following conditions:
	 *  - if an unknown account type is encountered
	 *  - if the account or it's parent UIDs are invalid
	 *  - if the parent account does not exist (i.e., has not been previously
	 *    defined)
	 *
	 * If successful, the account is created and added to the `accounts` and
	 * `accountTypes` maps.
	 */
	void readVersion2Account();

	/**
	 * Converts the given account type as a string to an enumeration.
	 *
	 * If the given string does not match any known account types,
	 * the `UnknownAccountType` enumeration value is returned.
	 *
	 * @param[in] type account type as a string
	 * @return account type as an enumeration
	 */
	AccountType toType(const QString& type) const;

	/**
	 * Parses the current XML stream for a version 2.0.0 transaction definition.
	 *
	 * An error may be raised for the following conditions:
	 *  - the `date-posted` element contains an invalid date
	 *  - the `currency` element contains an invalid currency definition
	 *
	 * If the date-posted for the transaction is not within the valid start
	 * and end dates, then the transaction is ignored.
	 */
	void readVersion2Transaction();

	/**
	 * Parses the current XML stream for transaction split definitions.
	 *
	 * @return parsed transaction splits
	 */
	QHash<QUuid, TransactionSplit> readSplits();

	/**
	 * Parses the current XML stream for a transaction split definition.
	 *
	 * An error may be raised for the following conditions:
	 *  - the `value` element contains an invalid number
	 *
	 *  @return parsed transaction split
	 */
	TransactionSplit readSplit();

	/**
	 * Creates individual transactions for the given set of transaction
	 * splits.
	 *
	 * An error may be raised for the following conditions:
	 *  - the primary split could not be determined from the given splits
	 *  - the account for any split does not exist
	 *
	 * Transactions are created by first determining the primary split. If the
	 * primary split is a withdrawal from an expense account, then the transaction
	 * is a refund. For each non-primary split, an `ImportedTransaction` is created.
	 * When withdrawing from the primary and not a refund, the primary split's account
	 * is used as the created transaction's withdrawal account and the secondary split's
	 * account is used as the deposit account. For all other cases (i.e., a refund), the
	 * primary split's account is used as the deposit account and the secondary split's
	 * account is used as the withdrawal account. This results in all expense related
	 * transactions flowing in the same "direction" (i.e., $10 from asset to expense,
	 * -$20 from asset to expense).
	 *
	 * @param[in] datePosted transaction date-posted
	 * @param[in] currency   ISO4217 currency code
	 * @param[in] payee      transaction payee
	 * @param[in] splits     transaction splits
	 */
	void createTransactionsFromSplits(const QDate& datePosted, const QString& currency,
		const QString& payee, const QHash<QUuid, TransactionSplit>& splits);

	/**
	 * Determines the primary split from the given list of transaction splits.
	 *
	 * The primary split is defined as the sole split from which or into which
	 * all transfered amounts originate. In the simple case of two splits, the
	 * primary split is the one from which the amount is withdrawn. In the case
	 * of a single withdrawal and multiple deposits (e.g., a multi-expense-category
	 * purchase), the primary split is the one from which the amount was withdrawn
	 * (usually an asset, bank, credit card, etc.). In the case of a single deposit
	 * and multiple withdrawals (e.g., a multi-expense-category refund), the
	 * primary split is the one to which the amount is deposited.
	 *
	 * The primary split is found by searching for the only negative split transfer
	 * value. If multiple negative splits are found, then the only positive split
	 * is used. If multiple negative and positive splits are found, then an empty
	 * UUID is returned as the primary could not be determined. This case does not
	 * normally happen, and represents some kind of complicated transaction that
	 * we do not support. Splits with a zero transfer value are ignored in the search.
	 *
	 * @return determined primary split UUID
	 */
	QUuid findPrimary(const QHash<QUuid, TransactionSplit>& splits);

	/**
	 * Parses the current XML element stream for a date definition.
	 *
	 * An error may be raised for the following conditions:
	 *  - no `ts:date` elements are found in the stream for the current element
	 *  - the date value found is invalid
	 *
	 * @return parsed date
	 */
	QDate readDate();

	/**
	 * Parses the current XML element stream for an ISO4217 currency code.
	 *
	 * An error may be raised for the following conditions:
	 *  - no `cmdty:id` elements are found in the stream for the current element
	 *  - the `cmdty:space` element contains anything other than "ISO4217"
	 *
	 * @return parsed currency ISO4217 code
	 */
	QString readCurrency();

	/**
	 * Parses the current XML element stream for a fractional number value.
	 *
	 * An error may be raised for the following conditions:
	 *  - the current element does not contain a string of the form "x/y"
	 *  - the divisor is 0 (e.g., "100/0")
	 *
	 * @return parsed fractional number
	 */
	double readDivisibleNumber();
};

}

#endif //GNUCASHREADER_HPP
