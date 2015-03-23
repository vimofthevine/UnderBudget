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
#include "AccountTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::AccountTest)

namespace ub {

//------------------------------------------------------------------------------
void AccountTest::fullName_data()
{
	QTest::addColumn<QString>("ggpName");
	QTest::addColumn<QString>("gpName");
	QTest::addColumn<QString>("pName");
	QTest::addColumn<QString>("cName");
	QTest::addColumn<QString>("delim");
	QTest::addColumn<QString>("fullName");

	QTest::newRow("all-named") << "ggp" << "gp"
		<< "p" << "c" << ":" << "ggp:gp:p:c";
	QTest::newRow("empty-root") << "" << "gp"
		<< "p" << "c" << ":" << "gp:p:c";
	QTest::newRow("empty-parent") << "ggp" << "gp"
		<< "" << "c" << ":" << "ggp:gp::c";
	QTest::newRow("all-empty") << "" << ""
		<< "" << "" << ":" << "";
	QTest::newRow("alt-delim") << "ggp" << "gp"
		<< "p" << "c" << "/" << "ggp/gp/p/c";
}

//------------------------------------------------------------------------------
void AccountTest::fullName()
{
	QFETCH(QString, ggpName);
	QFETCH(QString, gpName);
	QFETCH(QString, pName);
	QFETCH(QString, cName);
	QFETCH(QString, delim);
	QFETCH(QString, fullName);

	Account* greatGrandParent = new Account(ggpName);
	Account* grandParent = new Account(gpName, QSharedPointer<Account>(greatGrandParent));
	Account* parent = new Account(pName, QSharedPointer<Account>(grandParent));
	Account child(cName, QSharedPointer<Account>(parent));

	QCOMPARE(child.fullName(delim), fullName);
}

//------------------------------------------------------------------------------
void AccountTest::equality_data()
{
	QTest::addColumn<QString>("parent1Name");
	QTest::addColumn<QString>("parent2Name");
	QTest::addColumn<bool>("child1Parent");
	QTest::addColumn<bool>("child2Parent");
	QTest::addColumn<QString>("child1Name");
	QTest::addColumn<QString>("child2Name");
	QTest::addColumn<bool>("equal");

	QTest::newRow("equal") << "p" << "p" << true << true
		<< "c" << "c" << true;
	QTest::newRow("names-differ") << "p" << "p" << true << true
		<< "c" << "d" << false;
	QTest::newRow("parents-differ") << "p" << "q" << true << true
		<< "c" << "c" << false;
	QTest::newRow("no-lhs-parent") << "p" << "p" << false << true
		<< "c" << "c" << false;
	QTest::newRow("no-rhs-parent") << "p" << "p" << true << false
		<< "c" << "c" << false;
	QTest::newRow("no-parents") << "p" << "q" << false << false
		<< "c" << "c" << true;
}

//------------------------------------------------------------------------------
void AccountTest::equality()
{
	QFETCH(QString, parent1Name);
	QFETCH(QString, parent2Name);
	QFETCH(bool, child1Parent);
	QFETCH(bool, child2Parent);
	QFETCH(QString, child1Name);
	QFETCH(QString, child2Name);
	QFETCH(bool, equal);

	QSharedPointer<Account> parent1(new Account(parent1Name));
	QSharedPointer<Account> parent2(new Account(parent2Name));

	Account child1(child1Name);
	Account child2(child1Name);

	if (child1Parent)
	{
		child1 = Account(child1Name, parent1);
	}

	if (child2Parent)
	{
		child2 = Account(child2Name, parent2);
	}

	QCOMPARE(child1 == child2, equal);
	QCOMPARE(child1 != child2, ! equal);
}

}

