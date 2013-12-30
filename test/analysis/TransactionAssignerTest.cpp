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
#include "accounting/Money.hpp"
#include "analysis/Actuals.hpp"
#include "analysis/Assignments.hpp"
#include "analysis/TransactionAssigner.hpp"
#include "budget/AssignmentRules.hpp"
#include "ledger/Account.hpp"
#include "ledger/ImportedTransaction.hpp"
#include "TransactionAssignerTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::TransactionAssignerTest)

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
static const uint NO_MATCH_TRN       = 1234;
static const uint ONE_MATCH_TRN      = 2234;
static const uint GEN_STR_MATCH_TRN  = 3234;
static const uint GEN_AMT_MATCH_TRN  = 4234;
static const uint GEN_DAT_MATCH_TRN  = 5234;
static const uint DATE_MATCH_TRN     = 6234;
static const uint AMT_MATCH_TRN      = 7234;
static const uint PAYEE_MATCH_TRN    = 8234;
static const uint MEMO_MATCH_TRN     = 9234;
static const uint DEPOSIT_MATCH_TRN  = 10234;
static const uint WITHDRAW_MATCH_TRN = 11234;
static const uint REFUND_TRN         = 12234;

//------------------------------------------------------------------------------
static const uint NO_MATCH_EST = 1111;
static const uint SPECIFIC_EST = 2222;
static const uint GENERIC_EST  = 3333;
static const uint DATE_EST     = 4444;
static const uint AMT_EST      = 5555;
static const uint PAYEE_EST    = 6666;
static const uint MEMO_EST     = 7777;
static const uint DEPOSIT_EST  = 8888;
static const uint WITHDRAW_EST = 9999;

//------------------------------------------------------------------------------
static const uint MULTI_COND_RULE = 1789;
static const uint GEN_STR_RULE    = 2789;
static const uint GEN_AMT_RULE    = 3789;
static const uint GEN_DAT_RULE    = 4789;
static const uint DATE_RULE       = 5789;
static const uint AMT_RULE        = 6789;
static const uint PAYEE_RULE      = 7789;
static const uint MEMO_RULE       = 8789;
static const uint DEPOSIT_RULE    = 9789;
static const uint WITHDRAW_RULE   = 10789;

//------------------------------------------------------------------------------
QSharedPointer<Account> account(QString name)
{
	return QSharedPointer<Account>(new Account(name));
}

//------------------------------------------------------------------------------
QList<ImportedTransaction> createTransactions()
{
	QList<ImportedTransaction> transactions;

	transactions
		<< ImportedTransaction(DEPOSIT_MATCH_TRN,  QDate(2013, 12, 1),   Money(1000, "USD"),   "Rent",           "",         account("rent"),   account("rent"))
		<< ImportedTransaction(GEN_AMT_MATCH_TRN,  QDate(2013, 12, 2),   Money(12.120, "USD"),  "Drink Shop",     "",         account("mybank"), account("dining"))
		<< ImportedTransaction(PAYEE_MATCH_TRN,    QDate(2013, 12, 8),   Money(1.99, "USD"),   "Hardware Store", "Key copy", account("mybank"), account("home"))
		<< ImportedTransaction(WITHDRAW_MATCH_TRN, QDate(2013, 12, 10),  Money(5000, "USD"),   "Loan",           "",         account("loan"),   account("mybank"))
		<< ImportedTransaction(AMT_MATCH_TRN,      QDate(2013, 12, 14),  Money(110.73, "USD"), "Car Repair",     "",         account("mybank"), account("auto"))
		<< ImportedTransaction(ONE_MATCH_TRN,      QDate(2013, 12, 18),  Money(45.23, "USD"),  "Dept Store",     "Gift",     account("mybank"), account("gifts"))
		<< ImportedTransaction(GEN_STR_MATCH_TRN,  QDate(2013, 12, 22),  Money(20.00, "USD"),     "Theater",        "",         account("mybank"), account("entertainment:movies"))
		<< ImportedTransaction(MEMO_MATCH_TRN,     QDate(2013, 12, 24),  Money(32.22, "USD"),  "Store",          "gifts",    account("cash"),   account("gift"))
		<< ImportedTransaction(DATE_MATCH_TRN,     QDate(2013, 12, 25),  Money(40, "USD"),     "Christmas Gift", "",         account("cash"),   account("gift"))
		<< ImportedTransaction(REFUND_TRN,         QDate(2013, 12, 26),  Money(-20, "USD"),    "Store",          "gifts",    account("cash"),   account("gift"))
		<< ImportedTransaction(NO_MATCH_TRN,       QDate(2013, 12, 28),  Money(60),            "",               "",         account("mybank"), account("gas"))
		<< ImportedTransaction(GEN_DAT_MATCH_TRN,  QDate(2013, 12, 31),  Money(4.00, "USD"),   "Parking",        "",         account("cash"),   account("misc"))
	;

	return transactions;
}

//------------------------------------------------------------------------------
QSharedPointer<AssignmentRules> createRules()
{
	QSharedPointer<AssignmentRules> rules = AssignmentRules::create();
	QList<AssignmentRule::Condition> conds;

	// Multiple conditions/specific estimate
	conds << AssignmentRule::Condition(AssignmentRule::Payee, AssignmentRule::Contains, true, "Store");
	conds << AssignmentRule::Condition(AssignmentRule::Date, AssignmentRule::Before, false, "2013-12-20");
	conds << AssignmentRule::Condition(AssignmentRule::Date, AssignmentRule::After, false, "2013-12-15");
	rules->createRule(MULTI_COND_RULE, SPECIFIC_EST, conds);

	// Memo rule (has to occur before payee rule)
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Memo, AssignmentRule::StringEquals, false, "gifts");
	rules->createRule(MEMO_RULE, MEMO_EST, conds);

	// Payee rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Payee, AssignmentRule::Contains, false, "store");
	rules->createRule(PAYEE_RULE, PAYEE_EST, conds);

	// Date rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Date, AssignmentRule::DateEquals, false, "2013-12-25");
	rules->createRule(DATE_RULE, DATE_EST, conds);

	// Deposit rule (has to occur before amount rule)
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::DepositAccount, AssignmentRule::StringEquals, false, "rent");
	rules->createRule(DEPOSIT_RULE, DEPOSIT_EST, conds);

	// Withdrawal rule (has to occur before amount rule)
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::WithdrawalAccount, AssignmentRule::StringEquals, false, "loan");
	rules->createRule(WITHDRAW_RULE, WITHDRAW_EST, conds);

	// Amount rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Amount, AssignmentRule::GreaterThan, false, "100,USD");
	rules->createRule(AMT_RULE, AMT_EST, conds);

	// Generic date rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Date, AssignmentRule::After, false, "2013-12-29");
	rules->createRule(GEN_DAT_RULE, GENERIC_EST, conds);

	// Generic string rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Payee, AssignmentRule::Contains, false, "e");
	rules->createRule(GEN_STR_RULE, GENERIC_EST, conds);

	// Generic amount rule
	conds.clear();
	conds << AssignmentRule::Condition(AssignmentRule::Amount, AssignmentRule::LessThan, false, "50,USD");
	rules->createRule(GEN_AMT_RULE, GENERIC_EST, conds);

	return rules;
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::actualsFromAssignment_data()
{
	QTest::addColumn<uint>("estimate");
	QTest::addColumn<Money>("amount");

	QTest::newRow("no-actuals")   << NO_MATCH_EST << Money();
	QTest::newRow("one-actual")   << SPECIFIC_EST << Money(45.23, "USD");
	QTest::newRow("many-actuals") << GENERIC_EST  << Money(36.12, "USD");
	QTest::newRow("refund")       << MEMO_EST     << Money(12.22, "USD");
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::actualsFromAssignment()
{
	QFETCH(uint, estimate);
	QFETCH(Money, amount);

	Actuals* actuals = new Actuals(this);
	Assignments* assignments = new Assignments(this);
	TransactionAssigner assigner(createRules(), assignments, actuals);
	assigner.assign(createTransactions());

	QCOMPARE(actuals->forEstimate(estimate), amount);
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::estimateAssociation_data()
{
	QTest::addColumn<uint>("transaction");
	QTest::addColumn<uint>("estimate");

	QTest::newRow("no-assignment")  << NO_MATCH_TRN       << (uint) 0;
	QTest::newRow("most-specific")  << ONE_MATCH_TRN      << SPECIFIC_EST;
	QTest::newRow("most-gen-str")   << GEN_STR_MATCH_TRN  << GENERIC_EST;
	QTest::newRow("most-gen-amt")   << GEN_AMT_MATCH_TRN  << GENERIC_EST;
	QTest::newRow("most-gen-dat")   << GEN_DAT_MATCH_TRN  << GENERIC_EST;
	QTest::newRow("date-match")     << DATE_MATCH_TRN     << DATE_EST;
	QTest::newRow("amount-match")   << AMT_MATCH_TRN      << AMT_EST;
	QTest::newRow("payee-match")    << PAYEE_MATCH_TRN    << PAYEE_EST;
	QTest::newRow("memo-match")     << MEMO_MATCH_TRN     << MEMO_EST;
	QTest::newRow("deposit-match")  << DEPOSIT_MATCH_TRN  << DEPOSIT_EST;
	QTest::newRow("withdraw-match") << WITHDRAW_MATCH_TRN << WITHDRAW_EST;
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::estimateAssociation()
{
	QFETCH(uint, transaction);
	QFETCH(uint, estimate);

	Actuals* actuals = new Actuals(this);
	Assignments* assignments = new Assignments(this);
	TransactionAssigner assigner(createRules(), assignments, actuals);
	assigner.assign(createTransactions());

	QCOMPARE(assignments->estimate(transaction), estimate);
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::ruleAssociation_data()
{
	QTest::addColumn<uint>("transaction");
	QTest::addColumn<uint>("rule");

	QTest::newRow("no-assignment")  << NO_MATCH_TRN       << (uint) 0;
	QTest::newRow("most-specific")  << ONE_MATCH_TRN      << MULTI_COND_RULE;
	QTest::newRow("most-gen-str")   << GEN_STR_MATCH_TRN  << GEN_STR_RULE;
	QTest::newRow("most-gen-amt")   << GEN_AMT_MATCH_TRN  << GEN_AMT_RULE;
	QTest::newRow("most-gen-dat")   << GEN_DAT_MATCH_TRN  << GEN_DAT_RULE;
	QTest::newRow("date-match")     << DATE_MATCH_TRN     << DATE_RULE;
	QTest::newRow("amount-match")   << AMT_MATCH_TRN      << AMT_RULE;
	QTest::newRow("payee-match")    << PAYEE_MATCH_TRN    << PAYEE_RULE;
	QTest::newRow("memo-match")     << MEMO_MATCH_TRN     << MEMO_RULE;
	QTest::newRow("deposit-match")  << DEPOSIT_MATCH_TRN  << DEPOSIT_RULE;
	QTest::newRow("withdraw-match") << WITHDRAW_MATCH_TRN << WITHDRAW_RULE;
}

//------------------------------------------------------------------------------
void TransactionAssignerTest::ruleAssociation()
{
	QFETCH(uint, transaction);
	QFETCH(uint, rule);

	Actuals* actuals = new Actuals(this);
	Assignments* assignments = new Assignments(this);
	TransactionAssigner assigner(createRules(), assignments, actuals);
	assigner.assign(createTransactions());

	QCOMPARE(assignments->rule(transaction), rule);
}

}

