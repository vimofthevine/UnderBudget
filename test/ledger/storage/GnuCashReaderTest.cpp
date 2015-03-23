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
#include "GnuCashReaderTest.hpp"
#include "gnucash_testdata.hpp"
#include "ledger/storage/GnuCashReader.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::GnuCashReaderTest)

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Money& money)
{
	QString str = QString("%1 (%2, %3)").arg(money.toString())
		.arg(QVariant(money.amount()).toString()).arg(money.currency().code());
	return toString(str);
}

}

namespace ub {

//------------------------------------------------------------------------------
void GnuCashReaderTest::success_data()
{
	QTest::addColumn<int>("index");
	QTest::addColumn<QDate>("date");
	QTest::addColumn<Money>("amount");
	QTest::addColumn<QString>("payee");
	QTest::addColumn<QString>("memo");
	QTest::addColumn<QString>("withdrawal");
	QTest::addColumn<QString>("deposit");

	int i = 0;

	QTest::newRow("before") << i++ << QDate(2013,10,31) << Money(56.72, "USD")
		<< QString("Grocer") << QString("")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Groceries");
	QTest::newRow("multi-movies") << i++ << QDate(2013,11,1) << Money(10, "USD")
		<< QString("Merchant") << QString("DVD")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Entertainment:Music/Movies");
	QTest::newRow("multi-groceries") << i++ << QDate(2013,11,1) << Money(28, "USD")
		<< QString("Merchant") << QString("Groceries")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Groceries");
	QTest::newRow("multi-housewares") << i++ << QDate(2013,11,1) << Money(48.89, "USD")
		<< QString("Merchant") << QString("Housewares")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Housewares");
	QTest::newRow("payday-federal") << i++ << QDate(2013,11,8) << Money(158.21, "USD")
		<< QString("Payday") << QString("")
		<< QString("Income:Salary")
		<< QString("Expenses:Taxes:Federal");
	QTest::newRow("payday-soc") << i++ << QDate(2013,11,8) << Money(94.21, "USD")
		<< QString("Payday") << QString("")
		<< QString("Income:Salary")
		<< QString("Expenses:Taxes:Social Security");
	QTest::newRow("payday-state") << i++ << QDate(2013,11,8) << Money(43.26, "USD")
		<< QString("Payday") << QString("")
		<< QString("Income:Salary")
		<< QString("Expenses:Taxes:State/Province");
	QTest::newRow("payday-bank") << i++ << QDate(2013,11,8) << Money(799.55, "USD")
		<< QString("Payday") << QString("Earned paycheck")
		<< QString("Income:Salary")
		<< QString("Assets:Current Assets:Bank Checking Account");
	QTest::newRow("payday-bank") << i++ << QDate(2013,11,12) << Money(80, "USD")
		<< QString("Cash from ATM") << QString("")
		<< QString("Assets:Current Assets:Bank Checking Account")
		<< QString("Assets:Current Assets:Cash in Wallet");
	QTest::newRow("refund") << i++ << QDate(2013,11,16) << Money(-65.43, "USD")
		<< QString("Merchant") << QString("Return lamp")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Housewares");
	QTest::newRow("gift") << i++ << QDate(2013,11,17) << Money(50.00, "USD")
		<< QString("Birthday gift") << QString("")
		<< QString("Income:Gifts Received")
		<< QString("Assets:Current Assets:Cash in Wallet");
	QTest::newRow("imbalance-groceries") << i++ << QDate(2013,11,20) << Money(6.50, "USD")
		<< QString("Merchant") << QString("")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Groceries");
	QTest::newRow("imbalance-housewares") << i++ << QDate(2013,11,20) << Money(32.09, "USD")
		<< QString("Merchant") << QString("")
		<< QString("Liabilities:Credit Card")
		<< QString("Expenses:Housewares");
	QTest::newRow("imbalance-housewares") << i++ << QDate(2013,11,20) << Money(0, "USD")
		<< QString("Merchant") << QString("")
		<< QString("Liabilities:Credit Card")
		<< QString("Imbalance-USD");
	QTest::newRow("payment") << i++ << QDate(2013,11,24) << Money(253.82, "USD")
		<< QString("CC Payment") << QString("")
		<< QString("Assets:Current Assets:Bank Checking Account")
		<< QString("Liabilities:Credit Card");
	QTest::newRow("last") << i++ << QDate(2013,11,30) << Money(17.83, "USD")
		<< QString("Ice Skating") << QString("")
		<< QString("Assets:Current Assets:Bank Checking Account")
		<< QString("Expenses:Entertainment:Recreation");
	QTest::newRow("after") << i++ << QDate(2013,12,1) << Money(6.40, "USD")
		<< QString("Restaurant") << QString("")
		<< QString("Assets:Current Assets:Bank Checking Account")
		<< QString("Expenses:Dining");
}

//------------------------------------------------------------------------------
void GnuCashReaderTest::success()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write((GnuCashTest::Header + GnuCashTest::FullAccountsList
		+ GnuCashTest::AllTransactions + GnuCashTest::Footer).toUtf8());
	buffer.seek(0);

	GnuCashReader reader;

	// Make sure import was successful
	QList<ImportedTransaction> transactions;
	bool success = reader.read(&buffer, transactions);
	if ( ! success)
		qDebug() << reader.errorString();
	QCOMPARE(success, true);

	QFETCH(int, index);
	QFETCH(QDate, date);
	QFETCH(Money, amount);
	QFETCH(QString, payee);
	QFETCH(QString, memo);
	QFETCH(QString, withdrawal);
	QFETCH(QString, deposit);

	// Make sure transaction was imported correctly
	QCOMPARE(transactions.size() > index, true);

	ImportedTransaction transaction = transactions.at(index);
	QCOMPARE(transaction.date(), date);
	QCOMPARE(transaction.amount(), amount);
	QCOMPARE(transaction.payee(), payee);
	QCOMPARE(transaction.memo(), memo);
	QCOMPARE(transaction.withdrawalAccount(), withdrawal);
	QCOMPARE(transaction.depositAccount(), deposit);
}

//------------------------------------------------------------------------------
void GnuCashReaderTest::noDateFilters()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write((GnuCashTest::Header + GnuCashTest::FullAccountsList
		+ GnuCashTest::AllTransactions + GnuCashTest::Footer).toUtf8());
	buffer.seek(0);

	GnuCashReader reader;
	QDate start(2013,11,1);
	QDate end(2013,11,30);

	// Make sure import was successful
	QList<ImportedTransaction> transactions;
	bool success = reader.read(&buffer, transactions);
	if ( ! success)
		qDebug() << reader.errorString();
	QCOMPARE(success, true);

	// Make sure first transaction is before the "start date"
	// (assumes test data includes transaction on such a date)
	ImportedTransaction transaction = transactions.at(0);
	QCOMPARE(transaction.date() < start, true);

	// Make sure last transaction is after the "end date"
	// (assumes test data includes transaction on such a date)
	transaction = transactions.at(transactions.size()-1);
	QCOMPARE(transaction.date() > end, true);
}

//------------------------------------------------------------------------------
void GnuCashReaderTest::withDateFilters()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write((GnuCashTest::Header + GnuCashTest::FullAccountsList
		+ GnuCashTest::AllTransactions + GnuCashTest::Footer).toUtf8());
	buffer.seek(0);

	GnuCashReader reader;
	QDate start(2013,11,1);
	QDate end(2013,11,30);

	// Make sure import was successful
	QList<ImportedTransaction> transactions;
	bool success = reader.read(&buffer, transactions, start, end);
	if ( ! success)
		qDebug() << reader.errorString();
	QCOMPARE(success, true);

	// Make sure first transaction is on the start date
	// (assumes test data includes transaction on that date)
	ImportedTransaction transaction = transactions.at(0);
	QCOMPARE(transaction.date(), start);

	// Make sure last transaction is on the end date
	// (assumes test data includes transaction on that date)
	transaction = transactions.at(transactions.size()-1);
	QCOMPARE(transaction.date(), end);
}

//------------------------------------------------------------------------------
void GnuCashReaderTest::errors_data()
{
	QTest::addColumn<QString>("xml");

	QTest::newRow("missing-parent-account") <<
		(GnuCashTest::Header + GnuCashTest::MissingParentAccountsList
		 + GnuCashTest::AllTransactions + GnuCashTest::Footer);
	QTest::newRow("no-clear-primary-split") <<
		(GnuCashTest::Header + GnuCashTest::FullAccountsList
		 + GnuCashTest::NonZeroImbalanceTrn + GnuCashTest::Footer);
	QTest::newRow("invalid-split-account") <<
		(GnuCashTest::Header + GnuCashTest::FullAccountsList
		 + GnuCashTest::InvalidAccountTrn + GnuCashTest::Footer);
}

//------------------------------------------------------------------------------
void GnuCashReaderTest::errors()
{
	QFETCH(QString, xml);

	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(xml.toUtf8());
	buffer.seek(0);

	GnuCashReader reader;

	// Make sure import was not successful
	QList<ImportedTransaction> transactions;
	bool success = reader.read(&buffer, transactions);
	if ( ! success)
		qDebug() << reader.errorString();
	QCOMPARE(success, false);
	QCOMPARE(transactions.size(), 0);
}

}

