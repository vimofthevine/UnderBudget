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

// Qt Include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/Session.hpp"
#include "ui/wizard/BudgetSourceWizard.hpp"

namespace ub {

//------------------------------------------------------------------------------
Session::Session(QWidget* parent)
	: QStackedWidget(parent)
{ }

//------------------------------------------------------------------------------
void Session::closeEvent(QCloseEvent* event)
{ }

//------------------------------------------------------------------------------
void Session::newBudget()
{
	isUntitled = true;
	setWindowTitle("New Budget[*]");
}

//------------------------------------------------------------------------------
bool Session::openBudget(QSharedPointer<BudgetSource> source)
{
	isUntitled = false;
	budgetSource = source;
	setWindowTitle(budgetName() + "[*]");
	return true;
}

//------------------------------------------------------------------------------
bool Session::save()
{
	if (isUntitled)
		return saveAs();
	else
		return save(budgetSource);
}

//------------------------------------------------------------------------------
bool Session::saveAs()
{
	QSharedPointer<BudgetSource> newSource =
		BudgetSourceWizard::promptForBudgetToSave(this, budgetSource);
	if (newSource.isNull())
		return false;
	return save(newSource);
}

//------------------------------------------------------------------------------
bool Session::save(const QSharedPointer<BudgetSource>& source)
{
	budgetSource = source;
	isUntitled = false;
	setWindowModified(false);
	setWindowTitle(budgetName() + "[*]");
	return true;
}

//------------------------------------------------------------------------------
bool Session::saveAsTemplate()
{
	return true;
}

//------------------------------------------------------------------------------
void Session::editBudget()
{
	emit redoAvailable(true);
	setWindowModified(true);
}

//------------------------------------------------------------------------------
void Session::editEstimates()
{
	emit undoAvailable(false);
	emit redoAvailable(true);
}

//------------------------------------------------------------------------------
void Session::editAssignmentRules()
{
	emit undoAvailable(true);
	emit redoAvailable(false);
}

//------------------------------------------------------------------------------
void Session::importTransactions()
{
	emit showProgress(0, 0);
}

//------------------------------------------------------------------------------
void Session::importTransactionsFrom()
{ }

//------------------------------------------------------------------------------
void Session::assignTransactions()
{
	emit showMessage("Transactions assigned");
	emit showProgress(66, 100);
}

//------------------------------------------------------------------------------
void Session::calculateBalances()
{
	emit showProgress(100, 100);
}

//------------------------------------------------------------------------------
void Session::showAnalysisSummary()
{ }

//------------------------------------------------------------------------------
void Session::showEstimateProgress()
{ }

//------------------------------------------------------------------------------
void Session::showEstimateImpact()
{ }

//------------------------------------------------------------------------------
void Session::showImportedTransactions()
{ }

//------------------------------------------------------------------------------
QString Session::budgetName() const
{
	if (budgetSource.isNull())
		return tr("New Budget");
	return tr("Budget %1").arg(budgetSource->location());
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetSource> Session::currentBudgetSource() const
{
	return budgetSource;
}

//------------------------------------------------------------------------------
bool Session::hasUndoableActions() const
{
	return true;
}

//------------------------------------------------------------------------------
bool Session::hasRedoableActions() const
{
	return false;
}

}

