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
#include <QtGui>

// UnderBudget include(s)
#include "ui/Session.hpp"

namespace ub {

//------------------------------------------------------------------------------
Session::Session(QWidget* parent)
	: QStackedWidget(parent)
{ }

//------------------------------------------------------------------------------
void Session::closeEvent(QCloseEvent* event)
{ }

//------------------------------------------------------------------------------
void Session::newBudgetFile()
{
	setWindowTitle("New Budget");
}

//------------------------------------------------------------------------------
bool Session::openBudgetFile(const QString& file)
{
	setWindowTitle(file);
	currentFile = file;
	return true;
}

//------------------------------------------------------------------------------
bool Session::save()
{
	return true;
}

//------------------------------------------------------------------------------
bool Session::saveAs()
{
	return true;
}

//------------------------------------------------------------------------------
bool Session::saveAsTemplate()
{
	return true;
}

//------------------------------------------------------------------------------
void Session::editBudget()
{ }

//------------------------------------------------------------------------------
void Session::editEstimates()
{ }

//------------------------------------------------------------------------------
void Session::editAssignmentRules()
{ }

//------------------------------------------------------------------------------
void Session::importTransactions()
{ }

//------------------------------------------------------------------------------
void Session::importTransactionsFrom()
{ }

//------------------------------------------------------------------------------
void Session::assignTransactions()
{
	emit showMessage("Transactions assigned");
}

//------------------------------------------------------------------------------
void Session::calculateBalances()
{ }

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
	return tr("Budget %1").arg(currentFile);
}

//------------------------------------------------------------------------------
QString Session::currentFileName() const
{
	return currentFile;
}

}

