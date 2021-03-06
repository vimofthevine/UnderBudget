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

#ifndef ESTIMATETEST_HPP
#define ESTIMATETEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

namespace ub {

/**
 * Unit tests for the Estimate class.
 */
class EstimateTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Creates a set of test estimates.
	 */
	void init();

	/**
	 * Tests that creating child estimates resets leaf-only fields in
	 * the parent estimate.
	 */
	void creatingChildResetsParentFields();

	/**
	 * Tests child counts of estimates.
	 */
	void childCount();

	/**
	 * Tests child retrieval of estimates.
	 */
	void childAt();

	/**
	 * Tests index retrieval of estimates.
	 */
	void indexOf();

	/**
	 * Tests parent retrieval of estimates.
	 */
	void parent();

	/**
	 * Tests ID-based retrieval of estimates.
	 */
	void find();

	/**
	 * Tests changing the name of an estimate.
	 */
	void changeName();

	/**
	 * Test data for changing the estimate name.
	 */
	void changeName_data();

	/**
	 * Tests changing the description of an estimate.
	 */
	void changeDescription();

	/**
	 * Test data for changing the estimate description.
	 */
	void changeDescription_data();

	/**
	 * Tests changing the type of an estimate.
	 */
	void changeType();

	/**
	 * Test data for changing the type of an estimate.
	 */
	void changeType_data();

	/**
	 * Tests changing the type of a parent estimate, which cascades
	 * down the tree.
	 */
	void changeTypeCascading();

	/**
	 * Tests changing the type of the root estimate, which cascades
	 * down the entire tree.
	 */
	void changeRootTypeCascading();

	/**
	 * Tests changing estimated amount of an estimate.
	 */
	void changeAmount();

	/**
	 * Test data for changing estimated amount.
	 */
	void changeAmount_data();

	/**
	 * Tests changing due date of an estimate.
	 */
	void changeDueDate();

	/**
	 * Test data for changing due date.
	 */
	void changeDueDate_data();

	/**
	 * Tests changing finished state of an estimate.
	 */
	void changeFinishedState();

	/**
	 * Test data for changing finished state.
	 */
	void changeFinishedState_data();

	/**
	 * Tests changing the finished state of a parent estimate, which cascades
	 * down the tree.
	 */
	void changeFinishedStateCascading();

	/**
	 * Tests changing the finished state of the root estimate, which cascades
	 * down the entire tree.
	 */
	void changeRootFinishedStateCascading();

	/**
	 * Tests adding of a new child to a leaf estimate.
	 */
	void addChildToLeaf();

	/**
	 * Tests adding of a new child to a parent estimate.
	 */
	void addAnotherChildToParent();

	/**
	 * Tests deleting of the last leaf estimate from a parent
	 * with multiple estimates.
	 */
	void deleteLastLeafFromParentOfMultiple();

	/**
	 * Tests deleting of the first leaf estimate from a parent
	 * with multiple estimates.
	 */
	void deleteFirstLeafFromParentOfMultiple();

	/**
	 * Tests deleting of the only leaf under a parent estimate.
	 */
	void deleteOnlyLeafFromParent();

	/**
	 * Tests deleting of a parent with a single child estimate.
	 */
	void deleteParentWithOneChild();

	/**
	 * Tests deleting of a parent of a parent estimate.
	 */
	void deleteParentOfParent();

	/**
	 * Tests reordering of estimates within the same parent.
	 */
	void reorderWithinParent();

	/**
	 * Tests moving of an estimate to another parent of the same type.
	 */
	void moveToAnotherParentOfSameType();

	/**
	 * Tests moving of an estimate to another parent of a different tyupe.
	 */
	void moveToAnotherParentOfDiffType();

	/**
	 * Tests moving of a parent estimate (sub-tree rebasing) to another
	 * parent of the same type.
	 */
	void moveParentToAnotherParentOfSameType();

	/**
	 * Tests moving of a parent estimate (sub-tree rebasing) to another
	 * parent of a different type.
	 */
	void moveParentToAnotherParentOfDiffType();

	/**
	 * Tests moving of an estimate to a leaf estimate, modifying the
	 * new parent to be parent-compatible.
	 */
	void moveToLeaf();

	/**
	 * Tests reordering of an estimate with an invalid negative index.
	 */
	void reorderWithInvalidNegativeIndex();

	/**
	 * Tests reordering of an estimate with an invalid positive (too large) index.
	 */
	void reorderWithInvalidPositiveIndex();

	/**
	 * Tests moving of an estimate with an invalid negative index.
	 */
	void moveWithInvalidNegativeIndex();

	/**
	 * Tests moving of an estimate with an invalid positive (too large) index.
	 */
	void moveWithInvalidPositiveIndex();

	/**
	 * Tests estimate progress with no actual data.
	 */
	void progressNoActuals();

	/**
	 * Test data for estimate progress with no actual data.
	 */
	void progressNoActuals_data();

	/**
	 * Tests estimate progress with some missing actual data.
	 */
	void progressSomeDataMissing();

	/**
	 * Test data for estimate progress with some missing actual data.
	 */
	void progressSomeDataMissing_data();

	/**
	 * Tests estimate progress with no missing actual data.
	 */
	void progressNoDataMissing();

	/**
	 * Test data for estimate progress with no missing actual data.
	 */
	void progressNoDataMissing_data();

	/**
	 * Tests estimate progress with extra actual data.
	 */
	void progressExtraData();

	/**
	 * Test data for estimate progress with extra actual data.
	 */
	void progressExtraData_data();

	/**
	 * Tests estimate progress with all actuals under estimated.
	 */
	void progressUnderEstimated();

	/**
	 * Test data for estimate progress with all actuals under estimated.
	 */
	void progressUnderEstimated_data();

	/**
	 * Tests estimate progress with all actuals over estimated.
	 */
	void progressOverEstimated();

	/**
	 * Test data for estimate progress with all actuals over estimated.
	 */
	void progressOverEstimated_data();

	/**
	 * Tests estimate progress with all actuals exactly as estimated.
	 */
	void progressAsEstimated();

	/**
	 * Test data for estimate progress with all actuals exactly as estimated.
	 */
	void progressAsEstimated_data();

	/**
	 * Tests estimate progress with due date notices.
	 */
	void progressDueDateNote();

	/**
	 * Test data for estimate progress with due date notices.
	 */
	void progressDueDateNote_data();

	/**
	 * Tests estimate progress with no due date notices.
	 */
	void progressNoDueDateNote();

	/**
	 * Test data for estimate progress with no due date notices.
	 */
	void progressNoDueDateNote_data();

	/**
	 * Tests balance impact with no actual data.
	 */
	void impactNoActuals();

	/**
	 * Test data for balance impact with no actual data.
	 */
	void impactNoActuals_data();

	/**
	 * Tests balance impact with some missing actual data.
	 */
	void impactSomeDataMissing();

	/**
	 * Test data for balance impact with some missing actual data.
	 */
	void impactSomeDataMissing_data();

	/**
	 * Tests balance impact with no missing actual data.
	 */
	void impactNoDataMissing();

	/**
	 * Test data for balance impact with no missing actual data.
	 */
	void impactNoDataMissing_data();

	/**
	 * Tests balance impact with extra actual data.
	 */
	void impactExtraData();

	/**
	 * Test data for balance impact with extra actual data.
	 */
	void impactExtraData_data();

	/**
	 * Tests balance impact with all actuals under estimated.
	 */
	void impactUnderEstimated();

	/**
	 * Test data for balance impact with all actuals under estimated.
	 */
	void impactUnderEstimated_data();

	/**
	 * Tests balance impact with all actuals over estimated.
	 */
	void impactOverEstimated();

	/**
	 * Test data for balance impact with all actuals over estimated.
	 */
	void impactOverEstimated_data();

	/**
	 * Tests balance impact with all actuals exactly as estimated.
	 */
	void impactAsEstimated();

	/**
	 * Test data for balance impact with all actuals exactly as estimated.
	 */
	void impactAsEstimated_data();

	/**
	 * Tests balance impact when finished.
	 */
	void impactWhenFinished();

	/**
	 * Test data for balance impact when finished.
	 */
	void impactWhenFinished_data();

private:
	// Test estimates
	QSharedPointer<Estimate> rootEstimate;
	Estimate* root;
	Estimate* expenses;
	Estimate* incomes;
	Estimate* salary;
	Estimate* utilities;
	Estimate* rent;
	Estimate* water;
	Estimate* food;
	Estimate* loan;
};

}

#endif //ESTIMATETEST_HPP
