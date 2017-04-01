/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
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

