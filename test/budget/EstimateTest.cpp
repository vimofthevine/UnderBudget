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

namespace ub {

//------------------------------------------------------------------------------
void EstimateTest::init()
{
	root = Estimate::createRoot();
	expenses = Estimate::create(root, 1111,
		"Expenses", "", Estimate::Expense, Money(12.0), QDate(2013,3,31), false);
	incomes = Estimate::create(root, 1222,
		"Incomes", "", Estimate::Income, Money(32.0), QDate(), true);
	salary = Estimate::create(incomes, 2333,
		"Salary", "", Estimate::Income, Money(100.0), QDate(), false);
	utilities = Estimate::create(expenses, 3444,
		"Utilities", "", Estimate::Expense, Money(50.0), QDate(), true);
	rent = Estimate::create(utilities, 4555,
		"Rent", "Apt.", Estimate::Expense, Money(500.0), QDate(2013,3,28), false);
	water = Estimate::create(utilities, 4666,
		"Water", "", Estimate::Expense, Money(25.34), QDate(2013,3,10), true);
	food = Estimate::create(expenses, 3555,
		"Food", "", Estimate::Expense, Money(120.0), QDate(), false);
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
	QCOMPARE(root->childCount(), 2);
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
	QCOMPARE(root->childAt(-1).isNull(), true);
	QCOMPARE(root->childAt(0).isNull(), false);
	QCOMPARE(root->childAt(1).isNull(), false);
	QCOMPARE(root->childAt(2).isNull(), true);

	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
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
	QCOMPARE(root->parentEstimate().isNull(), true);
	QCOMPARE(expenses->parentEstimate(), root);
	QCOMPARE(incomes->parentEstimate(), root);
	QCOMPARE(salary->parentEstimate(), incomes);
	QCOMPARE(utilities->parentEstimate(), expenses);
	QCOMPARE(rent->parentEstimate(), utilities);
	QCOMPARE(water->parentEstimate(), utilities);
	QCOMPARE(food->parentEstimate(), expenses);
}

//------------------------------------------------------------------------------
void EstimateTest::changeName_data()
{
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<QString>("newName");
	QTest::addColumn<QString>("preName");
	QTest::addColumn<QString>("postName");

	QTest::newRow("root") << root << QString("the root") << QString("Root") << QString("Root");
	QTest::newRow("parent") << utilities << QString("Utility bills") << QString("Utilities") << QString("Utility bills");
	QTest::newRow("leaf") << rent << QString("Apartment") << QString("Rent") << QString("Apartment");
}

//------------------------------------------------------------------------------
void EstimateTest::changeName()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(QString, newName);
	QFETCH(QString, preName);
	QFETCH(QString, postName);

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
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<QString>("newDescrip");
	QTest::addColumn<QString>("preDescrip");
	QTest::addColumn<QString>("postDescrip");

	QTest::newRow("root") << root << QString("the root") << QString("") << QString("");
	QTest::newRow("empty") << utilities << QString("Utility bills") << QString("") << QString("Utility bills");
	QTest::newRow("not-empty") << rent << QString("apartment") << QString("Apt.") << QString("apartment");
	QTest::newRow("clear") << rent << QString("") << QString("Apt.") << QString("");
}

//------------------------------------------------------------------------------
void EstimateTest::changeDescription()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(QString, newDescrip);
	QFETCH(QString, preDescrip);
	QFETCH(QString, postDescrip);

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
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<Estimate::Type>("newType");
	QTest::addColumn<Estimate::Type>("preType");
	QTest::addColumn<Estimate::Type>("postType");

	QTest::newRow("root") << root << Estimate::Expense << Estimate::Root << Estimate::Root;
	QTest::newRow("leaf") << food << Estimate::Income << Estimate::Expense << Estimate::Income;
	QTest::newRow("parent") << utilities << Estimate::Transfer << Estimate::Expense << Estimate::Transfer;
}

//------------------------------------------------------------------------------
void EstimateTest::changeType()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(Estimate::Type, newType);
	QFETCH(Estimate::Type, preType);
	QFETCH(Estimate::Type, postType);

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
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<Money>("newAmount");
	QTest::addColumn<Money>("preAmount");
	QTest::addColumn<Money>("postAmount");

	QTest::newRow("root") << root << Money(150.0) << Money() << Money();
	QTest::newRow("leaf-nonzero") << food << Money(150.0) << Money(120.0) << Money(150.0);
	QTest::newRow("leaf-zero") << food << Money() << Money(120.0) << Money();
	QTest::newRow("parent") << utilities << Money(150.0) << Money() << Money();
}

//------------------------------------------------------------------------------
void EstimateTest::changeAmount()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(Money, newAmount);
	QFETCH(Money, preAmount);
	QFETCH(Money, postAmount);

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
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<QDate>("newDate");
	QTest::addColumn<QDate>("preDate");
	QTest::addColumn<QDate>("postDate");

	QTest::newRow("root") << root << QDate(2013,4,1) << QDate() << QDate();
	QTest::newRow("leaf-nodate") << food << QDate(2013,4,1) << QDate() << QDate(2013,4,1);
	QTest::newRow("leaf-hasdate") << water << QDate(2013,4,1) << QDate(2013,3,10) << QDate(2013,4,1);
	QTest::newRow("leaf-cleardate") << water << QDate() << QDate(2013,3,10) << QDate();
	QTest::newRow("parent") << utilities << QDate(2013,4,1) << QDate() << QDate();
}

//------------------------------------------------------------------------------
void EstimateTest::changeDueDate()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(QDate, newDate);
	QFETCH(QDate, preDate);
	QFETCH(QDate, postDate);

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
	QTest::addColumn<QSharedPointer<Estimate> >("estimate");
	QTest::addColumn<bool>("newState");
	QTest::addColumn<bool>("preState");
	QTest::addColumn<bool>("postState");

	QTest::newRow("root") << root << true << false << false;
	QTest::newRow("leaf-unfinished") << food << true << false << true;
	QTest::newRow("leaf-finished") << water << false << true << false;
	QTest::newRow("parent") << utilities << true << false << false;
}

//------------------------------------------------------------------------------
void EstimateTest::changeFinishedState()
{
	QFETCH(QSharedPointer<Estimate>, estimate);
	QFETCH(bool, newState);
	QFETCH(bool, preState);
	QFETCH(bool, postState);

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
	QSharedPointer<Estimate> child = water->childAt(0);
	QCOMPARE(child.isNull(), false);
	// Verify that the new leaf inherited from the parent
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
	QSharedPointer<Estimate> child = utilities->childAt(2);
	QCOMPARE(child.isNull(), false);
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
	QSharedPointer<Estimate> child = utilities->childAt(1);
	QCOMPARE(child.isNull(), false);
	QCOMPARE(child->estimateId(), water->estimateId());
	QCOMPARE(child->estimateName(), water->estimateName());
	QCOMPARE(child->estimateDescription(), water->estimateDescription());
	QCOMPARE(child->estimateType(), water->estimateType());
	QCOMPARE(child->estimatedAmount(), water->estimatedAmount());
	QCOMPARE(child->activityDueDate(), water->activityDueDate());
	QCOMPARE(child->isActivityFinished(), water->isActivityFinished());
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
	QSharedPointer<Estimate> child = utilities->childAt(0);
	QCOMPARE(child.isNull(), false);
	QCOMPARE(child->estimateId(), rent->estimateId());
	QCOMPARE(child->estimateName(), rent->estimateName());
	QCOMPARE(child->estimateDescription(), rent->estimateDescription());
	QCOMPARE(child->estimateType(), rent->estimateType());
	QCOMPARE(child->estimatedAmount(), rent->estimatedAmount());
	QCOMPARE(child->activityDueDate(), rent->activityDueDate());
	QCOMPARE(child->isActivityFinished(), rent->isActivityFinished());
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
	QSharedPointer<Estimate> child = incomes->childAt(0);
	QCOMPARE(child.isNull(), false);
	QCOMPARE(child->estimateId(), salary->estimateId());
	QCOMPARE(child->estimateName(), salary->estimateName());
	QCOMPARE(child->estimateDescription(), salary->estimateDescription());
	QCOMPARE(child->estimateType(), salary->estimateType());
	QCOMPARE(child->estimatedAmount(), salary->estimatedAmount());
	QCOMPARE(child->activityDueDate(), salary->activityDueDate());
	QCOMPARE(child->isActivityFinished(), salary->isActivityFinished());
}

//------------------------------------------------------------------------------
void EstimateTest::deleteParentWithOneChild()
{
	QUndoCommand* cmd = incomes->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->estimatedAmount(), Money());
	QCOMPARE(salary->estimatedAmount(), Money(100.0));

	cmd->redo();
	QCOMPARE(root->childCount(), 1);
	QCOMPARE(root->childAt(0), expenses);

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(0), expenses);
	// Verify that parent re-created correctly
	QSharedPointer<Estimate> parent = root->childAt(1);
	QCOMPARE(parent.isNull(), false);
	QCOMPARE(parent->estimateId(), incomes->estimateId());
	QCOMPARE(parent->estimateName(), incomes->estimateName());
	QCOMPARE(parent->estimateDescription(), incomes->estimateDescription());
	QCOMPARE(parent->estimateType(), incomes->estimateType());
	QCOMPARE(parent->estimatedAmount(), incomes->estimatedAmount());
	QCOMPARE(parent->activityDueDate(), incomes->activityDueDate());
	QCOMPARE(parent->isActivityFinished(), incomes->isActivityFinished());
	QCOMPARE(parent->childCount(), 1);
	// Verify child re-created correctly
	QSharedPointer<Estimate> child = parent->childAt(0);
	QCOMPARE(child.isNull(), false);
	QCOMPARE(child->estimateId(), salary->estimateId());
	QCOMPARE(child->estimateName(), salary->estimateName());
	QCOMPARE(child->estimateDescription(), salary->estimateDescription());
	QCOMPARE(child->estimateType(), salary->estimateType());
	QCOMPARE(child->estimatedAmount(), salary->estimatedAmount());
	QCOMPARE(child->activityDueDate(), salary->activityDueDate());
	QCOMPARE(child->isActivityFinished(), salary->isActivityFinished());
}

//------------------------------------------------------------------------------
void EstimateTest::deleteParentOfParent()
{
	QUndoCommand* cmd = expenses->deleteEstimate();

	// Assert pre-conditions
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(0), expenses);
	QCOMPARE(root->childAt(1), incomes);
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);

	cmd->redo();
	QCOMPARE(root->childCount(), 1);
	QCOMPARE(root->childAt(0), incomes);

	cmd->undo();
	// Verify that original structure restored
	QCOMPARE(root->childCount(), 2);
	QCOMPARE(root->childAt(1), incomes);

	QSharedPointer<Estimate> reExpenses = root->childAt(0);
	QCOMPARE(reExpenses.isNull(), false);
	QCOMPARE(reExpenses->estimateId(), expenses->estimateId());
	QCOMPARE(reExpenses->estimateName(), expenses->estimateName());
	QCOMPARE(reExpenses->childCount(), 2);

	QSharedPointer<Estimate> reUtilities = reExpenses->childAt(0);
	QCOMPARE(reUtilities.isNull(), false);
	QCOMPARE(reUtilities->estimateId(), utilities->estimateId());
	QCOMPARE(reUtilities->estimateName(), utilities->estimateName());
	QCOMPARE(reUtilities->childCount(), 2);

	QSharedPointer<Estimate> reFood = reExpenses->childAt(1);
	QCOMPARE(reFood.isNull(), false);
	QCOMPARE(reFood->estimateId(), food->estimateId());
	QCOMPARE(reFood->estimateName(), food->estimateName());
	QCOMPARE(reFood->childCount(), 0);

	QSharedPointer<Estimate> reRent = reUtilities->childAt(0);
	QCOMPARE(reRent.isNull(), false);
	QCOMPARE(reRent->estimateId(), rent->estimateId());
	QCOMPARE(reRent->estimateName(), rent->estimateName());
	QCOMPARE(reRent->childCount(), 0);

	QSharedPointer<Estimate> reWater = reUtilities->childAt(1);
	QCOMPARE(reWater.isNull(), false);
	QCOMPARE(reWater->estimateId(), water->estimateId());
	QCOMPARE(reWater->estimateName(), water->estimateName());
	QCOMPARE(reWater->childCount(), 0);
}

//------------------------------------------------------------------------------
void EstimateTest::redorderWithinParent()
{
	QUndoCommand* cmd = water->moveTo(utilities, 0);

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify that the order was modified
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);
	// Verify that type didn't change
	QCOMPARE(water->estimateType(), Estimate::Expense);

	cmd->undo();
	// Verify that the order was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
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
	QCOMPARE(food->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify the estimate was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), food);
	QCOMPARE(utilities->childAt(2), water);
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
	QCOMPARE(food->estimateType(), Estimate::Expense);

	cmd->redo();
	// Verify the estimate was moved
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(incomes->childCount(), 2);
	QCOMPARE(incomes->childAt(0), salary);
	QCOMPARE(incomes->childAt(1), food);
	// Verify that the type was changed
	QCOMPARE(food->estimateType(), Estimate::Income);

	cmd->undo();
	// Verify the original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
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
	// Verify that the types were changed
	QCOMPARE(utilities->estimateType(), Estimate::Income);
	QCOMPARE(rent->estimateType(), Estimate::Income);
	QCOMPARE(water->estimateType(), Estimate::Income);

	cmd->undo();
	// Verify that original strucutre was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(incomes->childCount(), 1);
	QCOMPARE(incomes->childAt(0), salary);
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
	QCOMPARE(food->estimatedAmount(), Money(120.0));
	QCOMPARE(food->activityDueDate(), QDate(2013,3,15));
	QCOMPARE(food->isActivityFinished(), true);

	cmd->redo();
	// Verify estimate was moved
	QCOMPARE(food->childCount(), 1);
	QCOMPARE(food->childAt(0), water);
	QCOMPARE(utilities->childCount(), 1);
	QCOMPARE(utilities->childAt(0), rent);
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

	cmd->redo();
	// Verify estimate was moved to first child index
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
}

//------------------------------------------------------------------------------
void EstimateTest::reorderWithInvalidPositiveIndex()
{
	QUndoCommand* cmd = rent->moveTo(utilities, 4);

	// Assert pre-conditions
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);

	cmd->redo();
	// Verify estimate was moved to last child index
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), water);
	QCOMPARE(utilities->childAt(1), rent);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
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

	cmd->redo();
	// Verify estimate was moved to first child index
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), food);
	QCOMPARE(utilities->childAt(1), rent);
	QCOMPARE(utilities->childAt(2), water);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
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

	cmd->redo();
	// Verify estimate was moved to last child index
	QCOMPARE(expenses->childCount(), 1);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(utilities->childCount(), 3);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
	QCOMPARE(utilities->childAt(2), food);

	cmd->undo();
	// Verify original structure was restored
	QCOMPARE(expenses->childCount(), 2);
	QCOMPARE(expenses->childAt(0), utilities);
	QCOMPARE(expenses->childAt(1), food);
	QCOMPARE(utilities->childCount(), 2);
	QCOMPARE(utilities->childAt(0), rent);
	QCOMPARE(utilities->childAt(1), water);
}

//------------------------------------------------------------------------------
void EstimateTest::estimateProgress()
{
}

//------------------------------------------------------------------------------
void EstimateTest::estimateProgress_data()
{
}

//------------------------------------------------------------------------------
void EstimateTest::balanceImpact()
{
}

//------------------------------------------------------------------------------
void EstimateTest::balanceImpact_data()
{
}

}

