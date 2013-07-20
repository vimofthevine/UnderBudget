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
#include <QUndoCommand>

// UnderBudget include(s)
#include "BalanceTest.hpp"
#include "budget/Balance.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::BalanceTest)

namespace ub {

//------------------------------------------------------------------------------
void BalanceTest::contributorCount()
{
	QList<Balance::Contributor> contributors;
	QCOMPARE(Balance::create(contributors)->contributorCount(), 0);
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	QCOMPARE(Balance::create(contributors)->contributorCount(), 1);
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);
	QCOMPARE(Balance::create(contributors)->contributorCount(), 2);
}

//------------------------------------------------------------------------------
void BalanceTest::contributorAt()
{
	QList<Balance::Contributor> contributors;

	// Test out-of-bound attempts
	QCOMPARE(Balance::create(contributors)->contributorAt(0).name, QString(""));
	QCOMPARE(Balance::create(contributors)->contributorAt(-1).name, QString(""));
	QCOMPARE(Balance::create(contributors)->contributorAt(1).name, QString(""));

	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	QCOMPARE(Balance::create(contributors)->contributorAt(0).name, QString("Asset"));
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);
	QCOMPARE(Balance::create(contributors)->contributorAt(1).name, QString("Liability"));
}

//------------------------------------------------------------------------------
void BalanceTest::value()
{
	QList<Balance::Contributor> contributors;
	QCOMPARE(Balance::create(contributors)->value(), Money());
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	QCOMPARE(Balance::create(contributors)->value(), Money(100, "USD"));
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);
	QCOMPARE(Balance::create(contributors)->value(), Money(75, "USD"));
}

//------------------------------------------------------------------------------
void BalanceTest::createFromMoney()
{
	QSharedPointer<Balance> balance = Balance::create(Money(1234, "USD"));
	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(1234, "USD"));
	QCOMPARE(balance->contributorAt(0).increase, true);
}

//------------------------------------------------------------------------------
void BalanceTest::addContributorToBalanceWithNone()
{
	QList<Balance::Contributor> contributors;

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->addContributor();

	QCOMPARE(balance->contributorCount(), 0);

	cmd->redo();
	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money());

	cmd->undo();
	QCOMPARE(balance->contributorCount(), 0);
}

//------------------------------------------------------------------------------
void BalanceTest::addContributorToBalanceWithOne()
{
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->addContributor();

	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));

	cmd->redo();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money());

	cmd->undo();
	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
}

//------------------------------------------------------------------------------
void BalanceTest::addContributorToBalanceWithMany()
{
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->addContributor();

	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));

	cmd->redo();
	QCOMPARE(balance->contributorCount(), 3);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
	QCOMPARE(balance->contributorAt(2).amount, Money());

	cmd->undo();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
}

//------------------------------------------------------------------------------
void BalanceTest::removeContributorFromBalanceWithNone()
{
	QList<Balance::Contributor> contributors;

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->removeContributor(0);

	QCOMPARE(balance->contributorCount(), 0);
	cmd->redo();
	QCOMPARE(balance->contributorCount(), 0);
	cmd->undo();
	QCOMPARE(balance->contributorCount(), 0);
}

//------------------------------------------------------------------------------
void BalanceTest::removeContributorFromBalanceWithOne()
{
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->removeContributor(0);

	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	cmd->redo();
	QCOMPARE(balance->contributorCount(), 0);
	QCOMPARE(balance->contributorAt(0).amount, Money());
	cmd->undo();
	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
}

//------------------------------------------------------------------------------
void BalanceTest::removeContributorFromBalanceWithMany()
{
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->removeContributor(0);

	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
	cmd->redo();
	QCOMPARE(balance->contributorCount(), 1);
	QCOMPARE(balance->contributorAt(0).amount, Money(25, "USD"));
	cmd->undo();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
}

//------------------------------------------------------------------------------
void BalanceTest::updateContributor()
{
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Asset", Money(100, "USD"), true);
	contributors << Balance::Contributor("Liability", Money(25, "USD"), false);

	QSharedPointer<Balance> balance = Balance::create(contributors);
	QUndoCommand* cmd = balance->updateContributor(1,
		Balance::Contributor("Loan", Money(53.5, "USD"), true));

	// Assert pre-conditions
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).name, QString("Liability"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
	QCOMPARE(balance->contributorAt(1).increase, false);

	cmd->redo();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).name, QString("Loan"));
	QCOMPARE(balance->contributorAt(1).amount, Money(53.5, "USD"));
	QCOMPARE(balance->contributorAt(1).increase, true);

	cmd->undo();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).amount, Money(100, "USD"));
	QCOMPARE(balance->contributorAt(1).name, QString("Liability"));
	QCOMPARE(balance->contributorAt(1).amount, Money(25, "USD"));
	QCOMPARE(balance->contributorAt(1).increase, false);
}

}

