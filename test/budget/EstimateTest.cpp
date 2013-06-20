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
#include "EstimateTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::EstimateTest)

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Money& money)
{
	return toString(money.toString());
}

}

namespace ub {

// Typedef this so we can use it in QFETCH
typedef QHash<uint,Money> Actuals;

//------------------------------------------------------------------------------
static const uint ROOT = 0;
static const uint EXPENSES = 1111;
static const uint INCOMES = 1222;
static const uint SALARY = 2333;
static const uint UTILITIES = 3444;
static const uint RENT = 4555;
static const uint WATER = 4666;
static const uint FOOD = 3555;
static const uint LOAN = 5666;

//------------------------------------------------------------------------------
void EstimateTest::init()
{
	rootEstimate = Estimate::createRoot();
	root = rootEstimate.data();
	expenses = Estimate::create(root, EXPENSES,
		"Expenses", "", Estimate::Expense, Money(12.0), QDate(2013,3,31), false);
	incomes = Estimate::create(root, INCOMES,
		"Incomes", "", Estimate::Income, Money(32.0), QDate(), true);
	salary = Estimate::create(incomes, SALARY,
		"Salary", "", Estimate::Income, Money(100.0), QDate(), false);
	utilities = Estimate::create(expenses, UTILITIES,
		"Utilities", "", Estimate::Expense, Money(50.0), QDate(), true);
	rent = Estimate::create(utilities, RENT,
		"Rent", "Apt.", Estimate::Expense, Money(500.0), QDate(2013,3,28), false);
	water = Estimate::create(utilities, WATER,
		"Water", "", Estimate::Expense, Money(25.34), QDate(2013,3,10), true);
	food = Estimate::create(expenses, FOOD,
		"Food", "", Estimate::Expense, Money(120.0), QDate(), false);
	loan = Estimate::create(root, LOAN,
		"Loan Payment", "", Estimate::Transfer, Money(50.0), QDate(2013,3,20), false);
}

//------------------------------------------------------------------------------
void EstimateTest::creatingChildResetsParentFields()
{
	// Make sure becoming a parent overwrote certain fields
	QCOMPARE(expenses->activityDueDate().isNull(), true);
	QCOMPARE(incomes->isActivityFinished(), false);
	QCOMPARE(utilities->estimatedAmount().isZero(), true);

	// Make sure leaf nodes have leaf-only fields set
	QCOMPARE(rent->activityDueDate().isNull(), false);
	QCOMPARE(water->isActivityFinished(), true);
	QCOMPARE(food->estimatedAmount().isZero(), false);
}

//------------------------------------------------------------------------------
void EstimateTest::childCount()
{
	QCOMPARE(root->childCount(), 3);
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(salary->childCount(), 0);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(rent->childCount(), 0);
	QCOMPARE(water->childCount(), 0);
	QCOMPARE(food->childCount(), 0);
}

//------------------------------------------------------------------------------
void EstimateTest::childAt()
{
	// Test out-of-bound attempts
	QCOMPARE(root->childAt(-1) == 0, true);
	QCOMPARE(root->childAt(0) == 0, false);
	QCOMPARE(root->childAt(1) == 0, false);
	QCOMPARE(root->childAt(2) == 0, false);
	QCOMPARE(root->childAt(3) == 0, true);

	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(root->childAt(2), loan);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
}

//------------------------------------------------------------------------------
void EstimateTest::indexOf()
{
	QCOMPARE(root->indexOf(expenses), 0);
	QCOMPARE(root->indexOf(incomes), 1);
	QCOMPARE(incomes->indexOf(salary), 0);
	QCOMPARE(expenses->indexOf(food), 1);
	QCOMPARE(expenses->indexOf(utilities), 0);
	QCOMPARE(utilities->indexOf(rent), 0);
	QCOMPARE(utilities->indexOf(water), 1);
}

//------------------------------------------------------------------------------
void EstimateTest::parent()
{
	QCOMPARE(root->parentEstimate() == 0, true);
	QCOMPARE(expenses->parentEstimate(), root);
	QCOMPARE(incomes->parentEstimate(), root);
	QCOMPARE(salary->parentEstimate(), incomes);
	QCOMPARE(utilities->parentEstimate(), expenses);
	QCOMPARE(rent->parentEstimate(), utilities);
	QCOMPARE(water->parentEstimate(), utilities);
	QCOMPARE(food->parentEstimate(), expenses);
}

//------------------------------------------------------------------------------
void EstimateTest::find()
{
	QCOMPARE(root->find(ROOT), root);
	QCOMPARE(root->find(EXPENSES), expenses);
	QCOMPARE(root->find(INCOMES), incomes);
	QCOMPARE(root->find(SALARY), salary);
	QCOMPARE(root->find(UTILITIES), utilities);
	QCOMPARE(root->find(RENT), rent);
	QCOMPARE(root->find(WATER), water);
	QCOMPARE(root->find(FOOD), food);
	QCOMPARE(root->find(9) == 0, true);
}

//------------------------------------------------------------------------------
void EstimateTest::changeName_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<QString>("newName");
	QTest::addColumn<QString>("preName");
	QTest::addColumn<QString>("postName");

	QTest::newRow("root") << ROOT << QString("the root") << QString("Root") << QString("Root");
	QTest::newRow("parent") << UTILITIES << QString("Utility bills") << QString("Utilities") << QString("Utility bills");
	QTest::newRow("leaf") << RENT << QString("Apartment") << QString("Rent") << QString("Apartment");
}

//------------------------------------------------------------------------------
void EstimateTest::changeName()
{
	QFETCH(uint, estimateId);
	QFETCH(QString, newName);
	QFETCH(QString, preName);
	QFETCH(QString, postName);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeName(newName);

	QCOMPARE(estimate->estimateName(), preName);
	cmd->redo();
	QCOMPARE(estimate->estimateName(), postName);
	cmd->undo();
	QCOMPARE(estimate->estimateName(), preName);
}

//------------------------------------------------------------------------------
void EstimateTest::changeDescription_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<QString>("newDescrip");
	QTest::addColumn<QString>("preDescrip");
	QTest::addColumn<QString>("postDescrip");

	QTest::newRow("root") << ROOT << QString("the root") << QString("") << QString("");
	QTest::newRow("empty") << UTILITIES << QString("Utility bills") << QString("") << QString("Utility bills");
	QTest::newRow("not-empty") << RENT << QString("apartment") << QString("Apt.") << QString("apartment");
	QTest::newRow("clear") << RENT << QString("") << QString("Apt.") << QString("");
}

//------------------------------------------------------------------------------
void EstimateTest::changeDescription()
{
	QFETCH(uint, estimateId);
	QFETCH(QString, newDescrip);
	QFETCH(QString, preDescrip);
	QFETCH(QString, postDescrip);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeDescription(newDescrip);

	QCOMPARE(estimate->estimateDescription(), preDescrip);
	cmd->redo();
	QCOMPARE(estimate->estimateDescription(), postDescrip);
	cmd->undo();
	QCOMPARE(estimate->estimateDescription(), preDescrip);
}

//------------------------------------------------------------------------------
void EstimateTest::changeType_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Estimate::Type>("newType");
	QTest::addColumn<Estimate::Type>("preType");
	QTest::addColumn<Estimate::Type>("postType");

	QTest::newRow("root") << ROOT << Estimate::Expense << Estimate::Root << Estimate::Root;
	QTest::newRow("leaf") << FOOD << Estimate::Income << Estimate::Expense << Estimate::Income;
	QTest::newRow("parent") << UTILITIES << Estimate::Transfer << Estimate::Expense << Estimate::Transfer;
}

//------------------------------------------------------------------------------
void EstimateTest::changeType()
{
	QFETCH(uint, estimateId);
	QFETCH(Estimate::Type, newType);
	QFETCH(Estimate::Type, preType);
	QFETCH(Estimate::Type, postType);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeType(newType);

	QCOMPARE(estimate->estimateType(), preType);
	cmd->redo();
	QCOMPARE(estimate->estimateType(), postType);
	cmd->undo();
	QCOMPARE(estimate->estimateType(), preType);
}

//------------------------------------------------------------------------------
void EstimateTest::changeTypeCascading()
{
	QUndoCommand* cmd = expenses->changeType(Estimate::Income);

	QCOMPARE(expenses->estimateType(), Estimate::Expense);
	QCOMPARE(food->estimateType(), Estimate::Expense);
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);
	cmd->redo();
	QCOMPARE(expenses->estimateType(), Estimate::Income);
	QCOMPARE(food->estimateType(), Estimate::Income);
	QCOMPARE(utilities->estimateType(), Estimate::Income);
	QCOMPARE(rent->estimateType(), Estimate::Income);
	QCOMPARE(water->estimateType(), Estimate::Income);
	cmd->undo();
	QCOMPARE(expenses->estimateType(), Estimate::Expense);
	QCOMPARE(food->estimateType(), Estimate::Expense);
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::changeRootTypeCascading()
{
	QUndoCommand* cmd = root->changeType(Estimate::Transfer);

	QCOMPARE(root->estimateType(), Estimate::Root);
	QCOMPARE(incomes->estimateType(), Estimate::Income);
	QCOMPARE(salary->estimateType(), Estimate::Income);
	QCOMPARE(expenses->estimateType(), Estimate::Expense);
	QCOMPARE(food->estimateType(), Estimate::Expense);
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);
	cmd->redo();
	QCOMPARE(root->estimateType(), Estimate::Root);
	QCOMPARE(incomes->estimateType(), Estimate::Transfer);
	QCOMPARE(salary->estimateType(), Estimate::Transfer);
	QCOMPARE(expenses->estimateType(), Estimate::Transfer);
	QCOMPARE(food->estimateType(), Estimate::Transfer);
	QCOMPARE(utilities->estimateType(), Estimate::Transfer);
	QCOMPARE(rent->estimateType(), Estimate::Transfer);
	QCOMPARE(water->estimateType(), Estimate::Transfer);
	cmd->undo();
	QCOMPARE(root->estimateType(), Estimate::Root);
	QCOMPARE(incomes->estimateType(), Estimate::Income);
	QCOMPARE(salary->estimateType(), Estimate::Income);
	QCOMPARE(expenses->estimateType(), Estimate::Expense);
	QCOMPARE(food->estimateType(), Estimate::Expense);
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::changeAmount_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("newAmount");
	QTest::addColumn<Money>("preAmount");
	QTest::addColumn<Money>("postAmount");

	QTest::newRow("root") << ROOT << Money(150.0) << Money() << Money();
	QTest::newRow("leaf-nonzero") << FOOD << Money(150.0) << Money(120.0) << Money(150.0);
	QTest::newRow("leaf-zero") << FOOD << Money() << Money(120.0) << Money();
	QTest::newRow("parent") << UTILITIES << Money(150.0) << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::changeAmount()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, newAmount);
	QFETCH(Money, preAmount);
	QFETCH(Money, postAmount);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeAmount(newAmount);

	QCOMPARE(estimate->estimatedAmount(), preAmount);
	cmd->redo();
	QCOMPARE(estimate->estimatedAmount(), postAmount);
	cmd->undo();
	QCOMPARE(estimate->estimatedAmount(), preAmount);
}

//------------------------------------------------------------------------------
void EstimateTest::changeDueDate_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<QDate>("newDate");
	QTest::addColumn<QDate>("preDate");
	QTest::addColumn<QDate>("postDate");

	QTest::newRow("root") << ROOT << QDate(2013,4,1) << QDate() << QDate();
	QTest::newRow("leaf-nodate") << FOOD << QDate(2013,4,1) << QDate() << QDate(2013,4,1);
	QTest::newRow("leaf-hasdate") << WATER << QDate(2013,4,1) << QDate(2013,3,10) << QDate(2013,4,1);
	QTest::newRow("leaf-cleardate") << WATER << QDate() << QDate(2013,3,10) << QDate();
	QTest::newRow("parent") << UTILITIES << QDate(2013,4,1) << QDate() << QDate();
}

//------------------------------------------------------------------------------
void EstimateTest::changeDueDate()
{
	QFETCH(uint, estimateId);
	QFETCH(QDate, newDate);
	QFETCH(QDate, preDate);
	QFETCH(QDate, postDate);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeDueDate(newDate);

	QCOMPARE(estimate->activityDueDate(), preDate);
	cmd->redo();
	QCOMPARE(estimate->activityDueDate(), postDate);
	cmd->undo();
	QCOMPARE(estimate->activityDueDate(), preDate);
}

//------------------------------------------------------------------------------
void EstimateTest::changeFinishedState_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<bool>("newState");
	QTest::addColumn<bool>("preState");
	QTest::addColumn<bool>("postState");

	QTest::newRow("root") << ROOT << true << false << false;
	QTest::newRow("leaf-unfinished") << FOOD << true << false << true;
	QTest::newRow("leaf-finished") << WATER << false << true << false;
	QTest::newRow("parent") << UTILITIES << true << false << false;
}

//------------------------------------------------------------------------------
void EstimateTest::changeFinishedState()
{
	QFETCH(uint, estimateId);
	QFETCH(bool, newState);
	QFETCH(bool, preState);
	QFETCH(bool, postState);

	Estimate* estimate = (estimateId == 0) ? root : root->find(estimateId);
	QUndoCommand* cmd = estimate->changeFinishedState(newState);

	QCOMPARE(estimate->isActivityFinished(), preState);
	cmd->redo();
	QCOMPARE(estimate->isActivityFinished(), postState);
	cmd->undo();
	QCOMPARE(estimate->isActivityFinished(), preState);
}

//------------------------------------------------------------------------------
void EstimateTest::changeFinishedStateCascading()
{
	QUndoCommand* cmd = expenses->changeFinishedState(true);

	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), false);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), false);
	QCOMPARE(water->isActivityFinished(), true);
	cmd->redo();
	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), true);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), true);
	QCOMPARE(water->isActivityFinished(), true);
	cmd->undo();
	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), false);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), false);
	QCOMPARE(water->isActivityFinished(), true);
}

//------------------------------------------------------------------------------
void EstimateTest::changeRootFinishedStateCascading()
{
	QUndoCommand* cmd = root->changeFinishedState(true);

	QCOMPARE(root->isActivityFinished(), false);
	QCOMPARE(incomes->isActivityFinished(), false);
	QCOMPARE(salary->isActivityFinished(), false);
	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), false);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), false);
	QCOMPARE(water->isActivityFinished(), true);
	cmd->redo();
	QCOMPARE(root->isActivityFinished(), false);
	QCOMPARE(incomes->isActivityFinished(), false);
	QCOMPARE(salary->isActivityFinished(), true);
	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), true);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), true);
	QCOMPARE(water->isActivityFinished(), true);
	cmd->undo();
	QCOMPARE(root->isActivityFinished(), false);
	QCOMPARE(incomes->isActivityFinished(), false);
	QCOMPARE(salary->isActivityFinished(), false);
	QCOMPARE(expenses->isActivityFinished(), false);
	QCOMPARE(food->isActivityFinished(), false);
	QCOMPARE(utilities->isActivityFinished(), false);
	QCOMPARE(rent->isActivityFinished(), false);
	QCOMPARE(water->isActivityFinished(), true);
}

//------------------------------------------------------------------------------
void EstimateTest::addChildToLeaf()
{
	QUndoCommand* cmd = water->addChild();

	// Assert pre-conditions
	QCOMPARE(water->childCount(), 0);

	cmd->redo();
	QCOMPARE(water->childCount(), 1);
	// Verify that the old leaf became parent-compatible
	QCOMPARE(water->estimatedAmount(), Money());
	QCOMPARE(water->activityDueDate(), QDate());
	// Make sure new child exists
	Estimate* child = water->childAt(0);
	QCOMPARE(child == 0, false);
	// Verify that the new leaf inherited from the parent
	QCOMPARE(child->estimateName(), water->estimateName());
	QCOMPARE(child->estimateType(), Estimate::Expense);
	QCOMPARE(child->estimatedAmount(), Money(25.34));
	QCOMPARE(child->activityDueDate(), QDate(2013,3,10));

	cmd->undo();
	QCOMPARE(water->childCount(), 0);
	// Verify that the parent was restored to original condition
	QCOMPARE(water->estimatedAmount(), Money(25.34));
	QCOMPARE(water->activityDueDate(), QDate(2013,3,10));
}

//------------------------------------------------------------------------------
void EstimateTest::addAnotherChildToParent()
{
	QUndoCommand* cmd = utilities->addChild();

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());

	cmd->redo();
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	// Verify that the parent remained parent-compatible
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());
	// Make sure new child exists
	Estimate* child = utilities->childAt(2);
	QCOMPARE(child == 0, false);
	// Verify that the new leaf inherited from the parent
	QCOMPARE(child->estimateName(), utilities->estimateName());
	QCOMPARE(child->estimateType(), Estimate::Expense);
	QCOMPARE(child->estimatedAmount(), Money());
	QCOMPARE(child->activityDueDate(), QDate());

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	// Verify that the parent remained parent-compatible
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());
}

//------------------------------------------------------------------------------
void EstimateTest::deleteLastLeafFromParentOfMultiple()
{
	QUndoCommand* cmd = water->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());

	cmd->redo();
	QCOMPARE(utilities->childCount(), 1);
	QCOMPARE(utilities->childAt(0), rent);
	// Verify that nothing happened with the parent
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	// Verify that nothing happened with the parent
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());
	// Verify child re-created correctly
	Estimate* child = utilities->childAt(1);
	QCOMPARE(child == 0, false);
	QCOMPARE(child->estimateId(), WATER);
	QCOMPARE(child->estimateName(), QString("Water"));
	QCOMPARE(child->estimateDescription(), QString(""));
	QCOMPARE(child->estimateType(), Estimate::Expense);
	QCOMPARE(child->estimatedAmount(), Money(25.34));
	QCOMPARE(child->activityDueDate(), QDate(2013,3,10));
	QCOMPARE(child->isActivityFinished(), true);
}

//------------------------------------------------------------------------------
void EstimateTest::deleteFirstLeafFromParentOfMultiple()
{
	QUndoCommand* cmd = rent->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());

	cmd->redo();
	QCOMPARE(utilities->childCount(), 1);
	QCOMPARE(utilities->childAt(0), water);
	// Verify that nothing happened with the parent
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(1), water);
	// Verify that nothing happened with the parent
	QCOMPARE(utilities->estimatedAmount(), Money());
	QCOMPARE(utilities->activityDueDate(), QDate());
	// Verify child re-created correctly
	Estimate* child = utilities->childAt(0);
	QCOMPARE(child == 0, false);
	QCOMPARE(child->estimateId(), RENT);
	QCOMPARE(child->estimateName(), QString("Rent"));
	QCOMPARE(child->estimateDescription(), QString("Apt."));
	QCOMPARE(child->estimateType(), Estimate::Expense);
	QCOMPARE(child->estimatedAmount(), Money(500.0));
	QCOMPARE(child->activityDueDate(), QDate(2013,3,28));
	QCOMPARE(child->isActivityFinished(), false);
}

//------------------------------------------------------------------------------
void EstimateTest::deleteOnlyLeafFromParent()
{
	QUndoCommand* cmd = salary->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->estimatedAmount(), Money());
	QCOMPARE(incomes->activityDueDate(), QDate());

	cmd->redo();
	QCOMPARE(incomes->childCount(), 0);
	// Verify that nothing happened with the parent
	QCOMPARE(incomes->estimatedAmount(), Money());
	QCOMPARE(incomes->activityDueDate(), QDate());

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(incomes->childCount(), 1);
	// Verify that nothing happened with the parent
	QCOMPARE(incomes->estimatedAmount(), Money());
	QCOMPARE(incomes->activityDueDate(), QDate());
	// Verify child re-created correctly
	Estimate* child = incomes->childAt(0);
	QCOMPARE(child == 0, false);
	QCOMPARE(child->estimateId(), SALARY);
	QCOMPARE(child->estimateName(), QString("Salary"));
	QCOMPARE(child->estimateDescription(), QString(""));
	QCOMPARE(child->estimateType(), Estimate::Income);
	QCOMPARE(child->estimatedAmount(), Money(100.0));
	QCOMPARE(child->activityDueDate(), QDate());
	QCOMPARE(child->isActivityFinished(), false);
}

//------------------------------------------------------------------------------
void EstimateTest::deleteParentWithOneChild()
{
	QUndoCommand* cmd = incomes->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(root->childCount(), 3);
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(root->childAt(2), loan);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->estimatedAmount(), Money());
	QCOMPARE(salary->estimatedAmount(), Money(100.0));

	cmd->redo();
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), loan);

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(2), loan);
	// Verify that parent re-created correctly
	Estimate* parent = root->childAt(1);
	QCOMPARE(parent == 0, false);
	QCOMPARE(parent->estimateId(), INCOMES);
	QCOMPARE(parent->estimateName(), QString("Incomes"));
	QCOMPARE(parent->estimateDescription(), QString(""));
	QCOMPARE(parent->estimateType(), Estimate::Income);
	QCOMPARE(parent->estimatedAmount(), Money());
	QCOMPARE(parent->activityDueDate(), QDate());
	QCOMPARE(parent->isActivityFinished(), false);
	QCOMPARE(parent->childCount(), 1);
	// Verify child re-created correctly
	Estimate* child = parent->childAt(0);
	QCOMPARE(child == 0, false);
	QCOMPARE(child->estimateId(), SALARY);
	QCOMPARE(child->estimateName(), QString("Salary"));
	QCOMPARE(child->estimateDescription(), QString(""));
	QCOMPARE(child->estimateType(), Estimate::Income);
	QCOMPARE(child->estimatedAmount(), Money(100.0));
	QCOMPARE(child->activityDueDate(), QDate());
	QCOMPARE(child->isActivityFinished(), false);
}

//------------------------------------------------------------------------------
void EstimateTest::deleteParentOfParent()
{
	QUndoCommand* cmd = expenses->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(root->childCount(), 3);
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(root->childAt(2), loan);
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);

	cmd->redo();
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(0), incomes);
	QCOMPARE(root->childAt(1), loan);

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(root->childCount(), 3);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(root->childAt(2), loan);

	Estimate* reExpenses = root->childAt(0);
	QCOMPARE(reExpenses == 0, false);
	QCOMPARE(reExpenses->estimateId(), EXPENSES);
	QCOMPARE(reExpenses->estimateName(), QString("Expenses"));
	QCOMPARE(reExpenses->childCount(), 2);

	Estimate* reUtilities = reExpenses->childAt(0);
	QCOMPARE(reUtilities == 0, false);
	QCOMPARE(reUtilities->estimateId(), UTILITIES);
	QCOMPARE(reUtilities->estimateName(), QString("Utilities"));
	QCOMPARE(reUtilities->childCount(), 2);

	Estimate* reFood = reExpenses->childAt(1);
	QCOMPARE(reFood == 0, false);
	QCOMPARE(reFood->estimateId(), FOOD);
	QCOMPARE(reFood->estimateName(), QString("Food"));
	QCOMPARE(reFood->childCount(), 0);

	Estimate* reRent = reUtilities->childAt(0);
	QCOMPARE(reRent == 0, false);
	QCOMPARE(reRent->estimateId(), RENT);
	QCOMPARE(reRent->estimateName(), QString("Rent"));
	QCOMPARE(reRent->childCount(), 0);

	Estimate* reWater = reUtilities->childAt(1);
	QCOMPARE(reWater == 0, false);
	QCOMPARE(reWater->estimateId(), WATER);
	QCOMPARE(reWater->estimateName(), QString("Water"));
	QCOMPARE(reWater->childCount(), 0);
}

//------------------------------------------------------------------------------
void EstimateTest::reorderWithinParent()
{
	QUndoCommand* cmd = water->moveTo(utilities, 0);

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify that the order was modified
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);
	QCOMPARE(root->find(WATER), water);
	// Verify that type didn't change
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->undo();
	// Verify that the order was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);
	// Verify that type didn't change
	QCOMPARE(water->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::moveToAnotherParentOfSameType()
{
	QUndoCommand* cmd = food->moveTo(utilities, 1);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);
	QCOMPARE(food->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify the estimate was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), food);
	QCOMPARE(utilities->childAt(2), water);
	QCOMPARE(root->find(FOOD), food);
	// Verify that the type didn't change
	QCOMPARE(food->estimateType(), Estimate::Expense);

	cmd->undo();
	// Verify the original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);
	// Verify that the type didn't change
	QCOMPARE(food->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::moveToAnotherParentOfDiffType()
{
	QUndoCommand* cmd = food->moveTo(incomes, 1);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(root->find(FOOD), food);
	QCOMPARE(food->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify the estimate was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(incomes->childCount(), 2);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->childAt(1), food);
	QCOMPARE(root->find(FOOD), food);
	// Verify that the type was changed
	QCOMPARE(food->estimateType(), Estimate::Income);

	cmd->undo();
	// Verify the original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(root->find(FOOD), food);
	// Verify that the type was restored
	QCOMPARE(food->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::moveParentToAnotherParentOfSameType()
{
	QUndoCommand* cmd = utilities->moveTo(food, 0);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(food->childCount(), 0);
	QCOMPARE(root->find(UTILITIES), utilities);
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify that estimate tree was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), food);
	QCOMPARE(food->childCount(), 1);
	QCOMPARE(food->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(UTILITIES), utilities);
	// Verifiy that type didn't change
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->undo();
	// Verify that original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(food->childCount(), 0);
	QCOMPARE(root->find(UTILITIES), utilities);
	// Verifiy that type didn't change
	QCOMPARE(water->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::moveParentToAnotherParentOfDiffType()
{
	QUndoCommand* cmd = utilities->moveTo(incomes, 1);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(root->find(UTILITIES), utilities);
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify that estimate tree was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), food);
	QCOMPARE(incomes->childCount(), 2);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->childAt(1), utilities);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(UTILITIES), utilities);
	// Verify that the types were changed
	QCOMPARE(utilities->estimateType(), Estimate::Income);
	QCOMPARE(rent->estimateType(), Estimate::Income);
	QCOMPARE(water->estimateType(), Estimate::Income);

	cmd->undo();
	// Verify that original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(root->find(UTILITIES), utilities);
	// Verify that original types were restored
	QCOMPARE(utilities->estimateType(), Estimate::Expense);
	QCOMPARE(rent->estimateType(), Estimate::Expense);
	QCOMPARE(water->estimateType(), Estimate::Expense);
}

//------------------------------------------------------------------------------
void EstimateTest::moveToLeaf()
{
	food->changeDueDate(QDate(2013,3,15))->redo();
	food->changeFinishedState(true)->redo();
	QUndoCommand* cmd = water->moveTo(food, 0);

	// Assert pre-conditions
	QCOMPARE(food->childCount(), 0);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);
	QCOMPARE(food->estimatedAmount(), Money(120.0));
	QCOMPARE(food->activityDueDate(), QDate(2013,3,15));
	QCOMPARE(food->isActivityFinished(), true);

	cmd->redo();
	// Verify estimate was moved
	QCOMPARE(food->childCount(), 1);
	QCOMPARE(food->childAt(0), water);
	QCOMPARE(utilities->childCount(), 1);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(root->find(WATER), water);
	// Verify new parent was made parent-compatible
	QCOMPARE(food->estimatedAmount(), Money());
	QCOMPARE(food->activityDueDate(), QDate());
	QCOMPARE(food->isActivityFinished(), false);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(food->childCount(), 0);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);
	// Verify parent returned to leaf
	QCOMPARE(food->estimatedAmount(), Money(120.0));
	QCOMPARE(food->isActivityFinished(), true);
	QCOMPARE(food->activityDueDate(), QDate(2013,3,15));
}

//------------------------------------------------------------------------------
void EstimateTest::reorderWithInvalidNegativeIndex()
{
	QUndoCommand* cmd = water->moveTo(utilities, -1);

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);

	cmd->redo();
	// Verify estimate was moved to first child index
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);
	QCOMPARE(root->find(WATER), water);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(WATER), water);
}

//------------------------------------------------------------------------------
void EstimateTest::reorderWithInvalidPositiveIndex()
{
	QUndoCommand* cmd = rent->moveTo(utilities, 4);

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(RENT), rent);

	cmd->redo();
	// Verify estimate was moved to last child index
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);
	QCOMPARE(root->find(RENT), rent);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(RENT), rent);
}

//------------------------------------------------------------------------------
void EstimateTest::moveWithInvalidNegativeIndex()
{
	QUndoCommand* cmd = food->moveTo(utilities, -1);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);

	cmd->redo();
	// Verify estimate was moved to first child index
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), food);
	QCOMPARE(utilities->childAt(1), rent);
	QCOMPARE(utilities->childAt(2), water);
	QCOMPARE(root->find(FOOD), food);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);
}

//------------------------------------------------------------------------------
void EstimateTest::moveWithInvalidPositiveIndex()
{
	QUndoCommand* cmd = food->moveTo(utilities, 6);

	// Assert pre-conditions
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);

	cmd->redo();
	// Verify estimate was moved to last child index
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(utilities->childAt(2), food);
	QCOMPARE(root->find(FOOD), food);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(root->find(FOOD), food);
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoActuals_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");

	QTest::newRow("root") << ROOT << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money();
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money();
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money();
	QTest::newRow("rent") << RENT << Money(500.0) << Money();
	QTest::newRow("water") << WATER << Money(25.34) << Money();
	QTest::newRow("food") << FOOD << Money(120.0) << Money();
	QTest::newRow("loan") << LOAN << Money(50.0) << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoActuals()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);

	QHash<uint, Money> actuals;

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
}

//------------------------------------------------------------------------------
void EstimateTest::progressSomeDataMissing_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");

	QTest::newRow("root") << ROOT << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money();
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(620.0);
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(500.0);
	QTest::newRow("rent") << RENT << Money(500.0) << Money(500.0);
	QTest::newRow("water") << WATER << Money(25.34) << Money();
	QTest::newRow("food") << FOOD << Money(120.0) << Money(120.0);
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(50.0);
}

//------------------------------------------------------------------------------
void EstimateTest::progressSomeDataMissing()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);

	QHash<uint, Money> actuals;
	actuals.insert(RENT, Money(500.0));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoDataMissing_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");

	QTest::newRow("root") << ROOT << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money(100.0);
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(645.34);
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(525.34);
	QTest::newRow("rent") << RENT << Money(500.0) << Money(500.0);
	QTest::newRow("water") << WATER << Money(25.34) << Money(25.34);
	QTest::newRow("food") << FOOD << Money(120.0) << Money(120.0);
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(50.0);
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoDataMissing()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
}

//------------------------------------------------------------------------------
void EstimateTest::progressExtraData_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");

	QTest::newRow("root") << ROOT << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money(100.0);
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(645.34);
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(525.34);
	QTest::newRow("rent") << RENT << Money(500.0) << Money(500.0);
	QTest::newRow("water") << WATER << Money(25.34) << Money(25.34);
	QTest::newRow("food") << FOOD << Money(120.0) << Money(120.0);
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(50.0);
}

//------------------------------------------------------------------------------
void EstimateTest::progressExtraData()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);

	QHash<uint, Money> actuals;
	actuals.insert(ROOT, Money(6234.0));
	actuals.insert(INCOMES, Money(6234.0));
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(EXPENSES, Money(6234.0));
	actuals.insert(UTILITIES, Money(6234.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
}

//------------------------------------------------------------------------------
void EstimateTest::progressUnderEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<bool>("isHealthy");

	QTest::newRow("root") << ROOT << Money() << Money() << true;
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money(90.0) << false;
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(90.0) << false;
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(623.34) << true;
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(513.34) << true;
	QTest::newRow("rent") << RENT << Money(500.0) << Money(490.0) << true;
	QTest::newRow("water") << WATER << Money(25.34) << Money(23.34) << true;
	QTest::newRow("food") << FOOD << Money(120.0) << Money(110.0) << true;
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(40.0) << true;
}

//------------------------------------------------------------------------------
void EstimateTest::progressUnderEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(bool, isHealthy);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(90.0));
	actuals.insert(RENT, Money(490.0));
	actuals.insert(WATER, Money(23.34));
	actuals.insert(FOOD, Money(110.0));
	actuals.insert(LOAN, Money(40.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
	QCOMPARE(progress.isHealthy, isHealthy);
}

//------------------------------------------------------------------------------
void EstimateTest::progressOverEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<bool>("isHealthy");

	QTest::newRow("root") << ROOT << Money() << Money() << true;
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money(110.0) << true;
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(110.0) << true;
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(667.34) << false;
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(537.34) << false;
	QTest::newRow("rent") << RENT << Money(500.0) << Money(510.0) << false;
	QTest::newRow("water") << WATER << Money(25.34) << Money(27.34) << false;
	QTest::newRow("food") << FOOD << Money(120.0) << Money(130.0) << false;
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(60.0) << false;
}

//------------------------------------------------------------------------------
void EstimateTest::progressOverEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(bool, isHealthy);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(110.0));
	actuals.insert(RENT, Money(510.0));
	actuals.insert(WATER, Money(27.34));
	actuals.insert(FOOD, Money(130.0));
	actuals.insert(LOAN, Money(60.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
	QCOMPARE(progress.isHealthy, isHealthy);
}

//------------------------------------------------------------------------------
void EstimateTest::progressAsEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<bool>("isHealthy");

	QTest::newRow("root") << ROOT << Money() << Money() << true;
	QTest::newRow("incomes") << INCOMES << Money(100.0) << Money(100.0) << true;
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0) << true;
	QTest::newRow("expenses") << EXPENSES << Money(645.34) << Money(645.34) << true;
	QTest::newRow("utilities") << UTILITIES << Money(525.34) << Money(525.34) << true;
	QTest::newRow("rent") << RENT << Money(500.0) << Money(500.0) << true;
	QTest::newRow("water") << WATER << Money(25.34) << Money(25.34) << true;
	QTest::newRow("food") << FOOD << Money(120.0) << Money(120.0) << true;
	QTest::newRow("loan") << LOAN << Money(50.0) << Money(50.0) << true;
}

//------------------------------------------------------------------------------
void EstimateTest::progressAsEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(bool, isHealthy);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.estimated, estimated);
	QCOMPARE(progress.actual, actual);
	QCOMPARE(progress.isHealthy, isHealthy);
}

//------------------------------------------------------------------------------
void EstimateTest::progressDueDateNote_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<QString>("note");

	QTest::newRow("root") << ROOT << "";
	QTest::newRow("incomes") << INCOMES << "";
	QTest::newRow("salary") << SALARY << "";
	QTest::newRow("expenses") << EXPENSES << "";
	QTest::newRow("utilities") << UTILITIES << "";
	QTest::newRow("rent") << RENT << tr("Due on %1").arg(QDate(2013,3,28).toString());
	QTest::newRow("water") << WATER << tr("Due on %1").arg(QDate(2013,3,10).toString());
	QTest::newRow("food") << FOOD << "";
	QTest::newRow("loan") << LOAN << tr("Due on %1").arg(QDate(2013,3,20).toString());
}

//------------------------------------------------------------------------------
void EstimateTest::progressDueDateNote()
{
	QFETCH(uint, estimateId);
	QFETCH(QString, note);

	QHash<uint, Money> actuals;

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.note, note);
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoDueDateNote_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<QString>("note");

	QTest::newRow("root") << ROOT << "";
	QTest::newRow("incomes") << INCOMES << "";
	QTest::newRow("salary") << SALARY << "";
	QTest::newRow("expenses") << EXPENSES << "";
	QTest::newRow("utilities") << UTILITIES << "";
	QTest::newRow("rent") << RENT << "";
	QTest::newRow("water") << WATER << "";
	QTest::newRow("food") << FOOD << "";
	QTest::newRow("loan") << LOAN << "";
}

//------------------------------------------------------------------------------
void EstimateTest::progressNoDueDateNote()
{
	QFETCH(uint, estimateId);
	QFETCH(QString, note);

	QHash<uint, Money> actuals;
	actuals.insert(RENT, Money(0.01));
	actuals.insert(WATER, Money(0.01));
	actuals.insert(LOAN, Money(0.01));

	Estimate* estimate = root->find(estimateId);
	Estimate::Progress progress = estimate->progress(actuals);
	QCOMPARE(progress.note, note);
}

//------------------------------------------------------------------------------
void EstimateTest::impactNoActuals_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money() << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money() << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money() << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money() << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactNoActuals()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	// Make sure all estimates are not finished
	root->changeFinishedState(false)->redo();

	QHash<uint, Money> actuals;

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactSomeDataMissing_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money() << Money(100.0);;
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-500.0) << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money() << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-120.0) << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactSomeDataMissing()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	// Make sure all estimates are not finished
	root->changeFinishedState(false)->redo();

	QHash<uint, Money> actuals;
	actuals.insert(RENT, Money(500.0));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactNoDataMissing_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-500.0) << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-25.34) << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-120.0) << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactNoDataMissing()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactExtraData_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-500.0) << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-25.34) << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-120.0) << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactExtraData()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	QHash<uint, Money> actuals;
	actuals.insert(ROOT, Money(6234.0));
	actuals.insert(INCOMES, Money(6234.0));
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(EXPENSES, Money(6234.0));
	actuals.insert(UTILITIES, Money(6234.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactUnderEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(90.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-490.0) << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-23.34) << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-110.0) << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactUnderEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	// Make sure all estimates are not finished
	root->changeFinishedState(false)->redo();

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(90.0));
	actuals.insert(RENT, Money(490.0));
	actuals.insert(WATER, Money(23.34));
	actuals.insert(FOOD, Money(110.0));
	actuals.insert(LOAN, Money(40.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactOverEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(110.0) << Money(110.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-510.0) << Money(-510.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-27.34) << Money(-27.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-130.0) << Money(-130.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactOverEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(110.0));
	actuals.insert(RENT, Money(510.0));
	actuals.insert(WATER, Money(27.34));
	actuals.insert(FOOD, Money(130.0));
	actuals.insert(LOAN, Money(60.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactAsEstimated_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(100.0) << Money(100.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-500.0) << Money(-500.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-25.34) << Money(-25.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-120.0) << Money(-120.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactAsEstimated()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(100.0));
	actuals.insert(RENT, Money(500.0));
	actuals.insert(WATER, Money(25.34));
	actuals.insert(FOOD, Money(120.0));
	actuals.insert(LOAN, Money(50.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

//------------------------------------------------------------------------------
void EstimateTest::impactWhenFinished_data()
{
	QTest::addColumn<uint>("estimateId");
	QTest::addColumn<Money>("estimated");
	QTest::addColumn<Money>("actual");
	QTest::addColumn<Money>("expected");

	QTest::newRow("root") << ROOT << Money() << Money() << Money();
	QTest::newRow("incomes") << INCOMES << Money() << Money() << Money();
	QTest::newRow("salary") << SALARY << Money(100.0) << Money(90.0) << Money(90.0);
	QTest::newRow("expenses") << EXPENSES << Money() << Money() << Money();
	QTest::newRow("utilities") << UTILITIES << Money() << Money() << Money();
	QTest::newRow("rent") << RENT << Money(-500.0) << Money(-490.0) << Money(-490.0);
	QTest::newRow("water") << WATER << Money(-25.34) << Money(-23.34) << Money(-23.34);
	QTest::newRow("food") << FOOD << Money(-120.0) << Money(-110.0) << Money(-110.0);
	QTest::newRow("loan") << LOAN << Money() << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::impactWhenFinished()
{
	QFETCH(uint, estimateId);
	QFETCH(Money, estimated);
	QFETCH(Money, actual);
	QFETCH(Money, expected);

	// Make sure all estimates are finished
	root->changeFinishedState(true)->redo();

	QHash<uint, Money> actuals;
	actuals.insert(SALARY, Money(90.0));
	actuals.insert(RENT, Money(490.0));
	actuals.insert(WATER, Money(23.34));
	actuals.insert(FOOD, Money(110.0));
	actuals.insert(LOAN, Money(40.0));

	Estimate* estimate = root->find(estimateId);
	Estimate::Impact impact = estimate->impact(actuals);
	QCOMPARE(impact.estimated, estimated);
	QCOMPARE(impact.actual, actual);
	QCOMPARE(impact.expected, expected);
}

}

