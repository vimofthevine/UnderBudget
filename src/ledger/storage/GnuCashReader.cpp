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
#include "gzip/GZipFile.hpp"
#include "ledger/Account.hpp"
#include "ledger/ImportedTransaction.hpp"
#include "ledger/storage/GnuCashReader.hpp"

namespace ub {

//------------------------------------------------------------------------------
GnuCashReader::GnuCashReader()
{ }

//------------------------------------------------------------------------------
GnuCashReader::GnuCashReader(const QString& fileName)
	: fileName(fileName)
{ }

//------------------------------------------------------------------------------
void GnuCashReader::import(const QDate& start, const QDate& end)
{
	if (fileName.isEmpty())
	{
		emit finished(ImportedTransactionSource::FailedWithError,
			tr("No file name specified."));
		return;
	}

	GZipFile gzipFile(fileName);
	if (gzipFile.open(QIODevice::ReadOnly))
	{
		QList<ImportedTransaction> trns;
		read(&gzipFile, trns, start, end);
	}
	else
	{
		QString msg = QFile(fileName).exists()
			? tr("File, %1, could not be opened.")
			: tr("File, %1, does not exist.");
		emit finished(ImportedTransactionSource::FailedWithError, msg);
	}
}

//------------------------------------------------------------------------------
QString GnuCashReader::errorString() const
{
	if (xml.error() == QXmlStreamReader::CustomError)
		return xml.errorString();
	else
	{
		return QObject::tr("%1\nLine %2, column %3")
			.arg(xml.errorString())
			.arg(xml.lineNumber())
			.arg(xml.columnNumber());
	}
}

//------------------------------------------------------------------------------
bool GnuCashReader::read(QIODevice* device, QList<ImportedTransaction>& trns,
	const QDate& start, const QDate& end)
{
	emit started();

	startDateFilter = start;
	endDateFilter = end;
	xml.setDevice(device);

	// Reset imported accounts and transactions
	accounts.clear();
	accountTypes.clear();
	transactions.clear();

	// Go through all top-level elements
	if (xml.readNextStartElement())
	{
		if (xml.name() == "gnc-v2")
		{
			readGncFile();
		}
		else
			xml.raiseError(tr("The given XML is not a valid GnuCash file."));
	}

	if (xml.hasError())
	{
		emit finished(ImportedTransactionSource::FailedWithError, errorString());
	}
	else
	{
		// sort the transactions
		qSort(transactions);

		emit finished(ImportedTransactionSource::Complete, "");
		emit imported(transactions);

		trns = transactions;
	}

	return ( ! xml.hasError());
}

//------------------------------------------------------------------------------
void GnuCashReader::readGncFile()
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "gnc-v2");
	bool found(false);

	// Need to find the gnc:book element
	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "gnc:book")
		{
			QString version = xml.attributes().value("version").toString();
			if (version == "2.0.0")
			{
				found = true;
				readVersion2Book();
			}
			else
				xml.raiseError(tr("Unsupported GnuCash version, %1").arg(version));
		}
		else
			xml.skipCurrentElement();
	}

	// Just in case we never found a gnc:book element
	if ( ! found)
		xml.raiseError(tr("XML is missing the GnuCash book element."));
}

//------------------------------------------------------------------------------
void GnuCashReader::readVersion2Book()
{
	Q_ASSERT(xml.isStartElement() && xml.qualifiedName() == "gnc:book"
		&& xml.attributes().value("version") == "2.0.0");

	// Go through all elements under the book
	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "gnc:account")
		{
			readVersion2Account();
		}
		else if (xml.qualifiedName() == "gnc:transaction")
		{
			readVersion2Transaction();
		}
		else
			xml.skipCurrentElement();
	}
}

//------------------------------------------------------------------------------
void GnuCashReader::readVersion2Account()
{
	Q_ASSERT(xml.isStartElement() && xml.qualifiedName() == "gnc:account"
		&& xml.attributes().value("version") == "2.0.0");

	QUuid uid;
	QUuid parentUid;
	QString name;
	QString typeStr;

	// Go through all elements under an account
	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "act:id")
		{
			uid = QUuid::createUuidV5(QUuid(), xml.readElementText());
		}
		else if (xml.qualifiedName() == "act:parent")
		{
			parentUid = QUuid::createUuidV5(QUuid(), xml.readElementText());
		}
		else if (xml.qualifiedName() == "act:name")
		{
			name = xml.readElementText();
		}
		else if (xml.qualifiedName() == "act:type")
		{
			typeStr = xml.readElementText();
		}
		else
			xml.skipCurrentElement();
	}

	//qDebug() << "Read account" << name << typeStr;

	AccountType type = toType(typeStr);

	if (type == UnknownAccountType)
	{
		xml.raiseError(tr("Unknown account type, %1.").arg(typeStr));
	}
	else if (uid.isNull())
	{
		xml.raiseError(tr("Missing account UID for %1.").arg(name));
	}
	else if (type == Root)
	{
		QSharedPointer<Account> root(new Account());
		accounts.insert(uid, root);
		accountTypes.insert(uid, type);
	}
	else if (parentUid.isNull())
	{
		xml.raiseError(tr("Missing parent UID for %1.").arg(name));
	}
	else
	{
		QSharedPointer<Account> parent = accounts.value(parentUid);
		if (parent.isNull())
		{
			xml.raiseError(tr("Missing parent account for %1.").arg(name));
		}
		else
		{
			QSharedPointer<Account> account(new Account(name, parent));
			accounts.insert(uid, account);
			accountTypes.insert(uid, type);
		}
	}
}

//------------------------------------------------------------------------------
GnuCashReader::AccountType GnuCashReader::toType(const QString& type) const
{
	QString upper = type.toUpper();

	if (upper == "ASSET")
		return Asset;
	else if (upper == "BANK")
		return Bank;
	else if (upper == "CASH")
		return Cash;
	else if (upper == "CREDIT")
		return Credit;
	else if (upper == "EQUITY")
		return Equity;
	else if (upper == "EXPENSE")
		return Expense;
	else if (upper == "INCOME")
		return Income;
	else if (upper == "LIABILITY")
		return Liability;
	else if (upper == "ROOT")
		return Root;

	return UnknownAccountType;
}

//------------------------------------------------------------------------------
void GnuCashReader::readVersion2Transaction()
{
	Q_ASSERT(xml.isStartElement() && xml.qualifiedName() == "gnc:transaction"
		&& xml.attributes().value("version") == "2.0.0");

	QUuid uid;
	QDate datePosted;
	QString currency("USD");
	QString payee;
	QHash<QUuid, TransactionSplit> splits;

	// Go through all elements under a transaction
	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "trn:id")
		{
			uid = QUuid::createUuidV5(QUuid(), xml.readElementText());
		}
		else if (xml.qualifiedName() == "trn:date-posted")
		{
			datePosted = readDate();
		}
		else if (xml.qualifiedName() == "trn:currency")
		{
			currency = readCurrency();
		}
		else if (xml.qualifiedName() == "trn:description")
		{
			payee = xml.readElementText();
		}
		else if (xml.qualifiedName() == "trn:splits")
		{
			splits = readSplits();
		}
		else
			xml.skipCurrentElement();
	}

	//qDebug() << "Read transaction" << datePosted << currency << payee;

	// Only filter by date if both start and end are valid dates
	if (startDateFilter.isValid() && endDateFilter.isValid())
	{
		// If before start or after end, skip this transaction
		if ((datePosted < startDateFilter) || (endDateFilter < datePosted))
			return;
	}

	createTransactionsFromSplits(datePosted, currency, payee, splits);
}

//------------------------------------------------------------------------------
QHash<QUuid, GnuCashReader::TransactionSplit> GnuCashReader::readSplits()
{
	Q_ASSERT(xml.isStartElement() && xml.qualifiedName() == "trn:splits");

	QHash<QUuid, TransactionSplit> splits;

	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "trn:split")
		{
			TransactionSplit split = readSplit();
			splits.insert(split.uid, split);
		}
		else
			xml.skipCurrentElement();
	}

	return splits;
}

//------------------------------------------------------------------------------
GnuCashReader::TransactionSplit GnuCashReader::readSplit()
{
	Q_ASSERT(xml.isStartElement() && xml.qualifiedName() == "trn:split");

	TransactionSplit split;

	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "split:id")
		{
			split.uid = QUuid::createUuidV5(QUuid(), xml.readElementText());
		}
		else if (xml.qualifiedName() == "split:memo")
		{
			split.memo = xml.readElementText();
		}
		else if (xml.qualifiedName() == "split:value")
		{
			split.value = readDivisibleNumber();
		}
		else if (xml.qualifiedName() == "split:account")
		{
			split.account = QUuid::createUuidV5(QUuid(), xml.readElementText());
		}
		else
			xml.skipCurrentElement();
	}

	//qDebug() << "Read split" << split.memo << split.value;

	return split;
}

//------------------------------------------------------------------------------
QDate GnuCashReader::readDate()
{
	Q_ASSERT(xml.isStartElement());

	QDate date;

	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "ts:date")
		{
			QString dateStr = xml.readElementText();
			date = QDate::fromString(dateStr, Qt::ISODate);

			if ( ! date.isValid())
				xml.raiseError(tr("Date string, %1, is invalid.").arg(dateStr));
		}
		else
			xml.skipCurrentElement();
	}

	if (date.isNull())
	{
		xml.raiseError(tr("Date string not found for %1.")
			.arg(xml.qualifiedName().toString()));
	}

	return date;
}

//------------------------------------------------------------------------------
QString GnuCashReader::readCurrency()
{
	Q_ASSERT(xml.isStartElement());

	QString iso4217;

	while (xml.readNextStartElement())
	{
		if (xml.qualifiedName() == "cmdty:id")
		{
			iso4217 = xml.readElementText();
		}
		else if (xml.qualifiedName() == "cmdty:space")
		{
			QString space = xml.readElementText();
			if (space != "ISO4217")
				xml.raiseError(tr("Unsupported commodity space, %1.").arg(space));
		}
		else
			xml.skipCurrentElement();
	}

	if (iso4217.isEmpty())
	{
		xml.raiseError(tr("Currency string not found for %1.")
			.arg(xml.qualifiedName().toString()));
	}

	return iso4217;
}

//------------------------------------------------------------------------------
double GnuCashReader::readDivisibleNumber()
{
	double quotient;

	QString valueStr = xml.readElementText();
	QStringList operands = valueStr.split('/');
	if (operands.length() != 2)
		xml.raiseError(tr("Invalid split value, %1.").arg(valueStr));
	else
	{
		double dividend = QVariant(operands.at(0)).toDouble();
		double divisor = QVariant(operands.at(1)).toDouble();

		if (divisor == 0.0)
			xml.raiseError(tr("Attempted division by 0, %1.").arg(valueStr));
		else
			quotient = dividend / divisor;
	}

	return quotient;
}

//------------------------------------------------------------------------------
void GnuCashReader::createTransactionsFromSplits(const QDate& datePosted,
	const QString& currency, const QString& payee,
	const QHash<QUuid, TransactionSplit>& splits)
{
	// Don't bother if an error exists
	if (xml.hasError())
		return;

	QUuid primaryUid = findPrimary(splits);
	if (primaryUid.isNull())
	{
		xml.raiseError(tr("Cannot determine primary split for transaction, %1, "
			"posted on %2.").arg(payee).arg(datePosted.toString()));
	}
	else
	{
		TransactionSplit primarySplit = splits.value(primaryUid);
		QSharedPointer<Account> primaryAccount = accounts.value(primarySplit.account);

		// Transaction is a refund if it's a withdrawal from an expense account
		bool primaryIsWithdrawal = (primarySplit.value < 0);
		bool primaryIsRefund = primaryIsWithdrawal
			&& (accountTypes.value(primarySplit.account, Root) == Expense);

		// Make sure account exists
		if (primaryAccount.isNull())
		{
			xml.raiseError(tr("Account does not exist for %1 split%2 for %3 posted on %4.")
				.arg(payee)
				.arg(primarySplit.memo.isEmpty() ? "" : " (" + primarySplit.memo + ")")
				.arg(primarySplit.value)
				.arg(datePosted.toString()));
			return;
		}

		QList<QUuid> uids = splits.keys();
		for (int i=0; i<uids.size(); ++i)
		{
			QUuid uid = uids.at(i);

			// Don't create a transaction for the primary split
			if (uid != primaryUid)
			{
				TransactionSplit split = splits.value(uid);
				QSharedPointer<Account> splitAccount = accounts.value(split.account);

				// Make sure account exists
				if (splitAccount.isNull())
				{
					xml.raiseError(tr("Account does not exist for %1 split%2 for %3 posted on %4.")
						.arg(payee)
						.arg(split.memo.isEmpty() ? "" : " (" + split.memo + ")")
						.arg(split.value)
						.arg(datePosted.toString()));
					return;
				}

				// If withdrawing from primary--but not a refund--then use primary for withdrawal
				QSharedPointer<Account> withdrawal = (primaryIsWithdrawal && ! primaryIsRefund)
					? primaryAccount : splitAccount;
				// Else (depositing into primary, or a refund) use primary for deposit
				QSharedPointer<Account> deposit = (primaryIsWithdrawal && ! primaryIsRefund)
					? splitAccount : primaryAccount;

				// If a refund, invert the transferred amount
				// (e.g., transfer -$20 from asset to expense)
				double transferAmount = primaryIsRefund ? (0.0 - split.value) : split.value;

				transactions.append(ImportedTransaction(qHash(split.uid), datePosted,
					Money(transferAmount, currency), payee, split.memo, withdrawal, deposit));
			}
		}
	}
}

//------------------------------------------------------------------------------
QUuid GnuCashReader::findPrimary(const QHash<QUuid, TransactionSplit>& splits)
{
	QUuid primary;
	const QList<TransactionSplit> values = splits.values();
	int negatives = 0;
	int positives = 0;
	QUuid positive;
	QUuid negative;

	// Go through each split, and record the number of positive and
	// negative transfer values as well as the last negative/positive split UIDs
	for (int i=0; i<values.size(); ++i)
	{
		TransactionSplit split = values.at(i);

		if (split.value == 0.0)
		{
			// Ignore empty splits
		}
		else if (split.value < 0)
		{
			negatives++;
			negative = split.uid;
		}
		else
		{
			positives++;
			positive = split.uid;
		}
	}

	// If only one negative transfer split, that's the primary
	// (if there was also only one positive split, this is still what we want)
	if (negatives == 1)
		primary = negative;
	else if (positives == 1)
		primary = positive;

	// If there were multiple negative and positive splits, then there is no
	// conclusive primary split

	return primary;
}

//------------------------------------------------------------------------------
void GnuCashReader::cancel()
{ }

}

