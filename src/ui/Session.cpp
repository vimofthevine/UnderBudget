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
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/BudgetDetailsForm.hpp"
#include "ui/budget/EstimateDisplayWidget.hpp"
#include "ui/budget/RulesListWidget.hpp"
#include "ui/ledger/ImportedTransactionsListWidget.hpp"
#include "ui/ledger/ImportedTransactionsModel.hpp"
#include "ui/wizard/BudgetSourceWizard.hpp"
#include "ui/wizard/TransactionSourceWizard.hpp"

namespace ub {

//------------------------------------------------------------------------------
Session::Session(QWidget* parent)
	: QStackedWidget(parent),
	  isUntitled(true)
{
	// Setup undo stack signals/slots
	undoStack = new QUndoStack(this);
	connect(undoStack, SIGNAL(cleanChanged(bool)),
		this, SLOT(setWindowModified(bool)));
	connect(undoStack, SIGNAL(canUndoChanged(bool)),
		this, SIGNAL(undoAvailable(bool)));
	connect(undoStack, SIGNAL(canRedoChanged(bool)),
		this, SIGNAL(redoAvailable(bool)));
}

//------------------------------------------------------------------------------
void Session::createWidgets()
{
	AssignmentRulesModel* rulesModel = new AssignmentRulesModel(budget->rules(),
		budget->estimates(), undoStack, this);
	transactionsModel = new ImportedTransactionsModel(this);

	budgetDetails = new BudgetDetailsForm(budget, undoStack, this);
	estimateDisplay = new EstimateDisplayWidget(budget->estimates(),
		rulesModel, undoStack, this);
	assignmentRules = new RulesListWidget(rulesModel, this);
	transactionsList = new ImportedTransactionsListWidget(transactionsModel, this);

	addWidget(budgetDetails);
	addWidget(estimateDisplay);
	addWidget(assignmentRules);
	addWidget(transactionsList);

	// Connect the selection of estimates and rules
	connect(assignmentRules, SIGNAL(estimateSelected(uint)),
		estimateDisplay, SLOT(selectEstimate(uint)));
	connect(transactionsList, SIGNAL(estimateSelected(uint)),
		estimateDisplay, SLOT(selectEstimate(uint)));
}

//------------------------------------------------------------------------------
void Session::updateWindowTitle()
{
	setWindowTitle(sessionName() + "[*]");
	emit titleChanged(windowTitle());
}

//------------------------------------------------------------------------------
void Session::setWindowModified(bool isClean)
{
	QStackedWidget::setWindowModified( ! isClean);
	emit budgetModified( ! isClean);
}

//------------------------------------------------------------------------------
void Session::closeEvent(QCloseEvent* event)
{
	if (promptToSave())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

//------------------------------------------------------------------------------
bool Session::promptToSave()
{
	if ( ! undoStack->isClean())
	{
		QMessageBox::StandardButton response;
		response = QMessageBox::warning(this, tr("Unsaved Changes"),
			tr("'%1' has been modified.\n"
			   "Do you want to save your changes?").arg(budgetName()),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if (response == QMessageBox::Save)
			return save();
		else if (response == QMessageBox::Cancel)
			return false;
		// Else discard
	}
	return true;
}

//------------------------------------------------------------------------------
void Session::newBudget()
{
	budget = QSharedPointer<Budget>(new Budget);
	isUntitled = true;
	connect(budget.data(), SIGNAL(nameChanged(QString)),
		this, SLOT(updateWindowTitle()));
	updateWindowTitle();
	createWidgets();
}

//------------------------------------------------------------------------------
bool Session::openBudget(QSharedPointer<BudgetSource> source)
{
	budgetSource = source;
	budget = source->retrieve();
	if ( ! budget)
	{
		QMessageBox::warning(this, tr("Error"), source->error());
		return false;
	}
	else
	{
		isUntitled = false;
		connect(budget.data(), SIGNAL(nameChanged(QString)),
			this, SLOT(updateWindowTitle()));
		updateWindowTitle();
		createWidgets();
		return true;
	}
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
	if (source->store(budget))
	{
		budgetSource = source;
		isUntitled = false;
		undoStack->setClean();
		updateWindowTitle();
		return true;
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), source->error());
		return false;
	}
}

//------------------------------------------------------------------------------
bool Session::saveAsTemplate()
{
	return true;
}

//------------------------------------------------------------------------------
void Session::editBudget()
{
	if (budget && budgetDetails)
	{
		setCurrentWidget(budgetDetails);
	}
}

//------------------------------------------------------------------------------
void Session::editEstimates()
{
	if (budget && estimateDisplay)
	{
		estimateDisplay->showEstimateDefinitions();
		setCurrentWidget(estimateDisplay);
	}
}

//------------------------------------------------------------------------------
void Session::editAssignmentRules()
{
	if (budget && assignmentRules)
	{
		setCurrentWidget(assignmentRules);
	}
}

//------------------------------------------------------------------------------
void Session::importTransactions()
{
	if (transactionSource)
	{
		importFromCurrentSource();
	}
	else
	{
		importTransactionsFrom();
	}
}

//------------------------------------------------------------------------------
void Session::importTransactionsFrom()
{
	QSharedPointer<ImportedTransactionSource> newSource =
		TransactionSourceWizard::promptForTransactionImport(this);

	if (newSource)
	{
		connect(newSource.data(), SIGNAL(started()),
			this, SLOT(importStarted()));
		connect(newSource.data(), SIGNAL(finished(ImportedTransactionSource::Result, QString)),
			this, SLOT(importFinished(ImportedTransactionSource::Result, QString)));
		connect(newSource.data(), SIGNAL(progress(int)),
			this, SLOT(importProgress(int)));
		connect(newSource.data(), SIGNAL(imported(QList<ImportedTransaction>)),
			this, SLOT(transactionsImported(QList<ImportedTransaction>)));

		transactionSource = newSource;
		importFromCurrentSource();
	}
}

//------------------------------------------------------------------------------
void Session::importFromCurrentSource()
{
	if (budget)
	{
		QDate start(budget->budgetingPeriod()->startDate());
		QDate end(budget->budgetingPeriod()->endDate());
		transactionSource->import(start, end);
	}
}

//------------------------------------------------------------------------------
void Session::importStarted()
{
	emit showMessage(tr("Importing transactions from %1")
		.arg(transactionSource->name()));

	// Start indefinite progress indicator
	emit showProgress(0, 0);
}

//------------------------------------------------------------------------------
void Session::importFinished(ImportedTransactionSource::Result result,
	const QString& message)
{
	// Clear progress indicator
	emit showProgress(100, 100);

	if (result == ImportedTransactionSource::Cancelled)
	{
		emit showMessage(tr("Import cancelled."));
	}
	else if (result == ImportedTransactionSource::FailedWithError)
	{
		QMessageBox::warning(this, tr("Error"), message);
	}
	else if ( ! message.isEmpty())
	{
		emit showMessage(message);
	}
}

//------------------------------------------------------------------------------
void Session::importProgress(int percent)
{
	emit showProgress(percent, 100);
}

//------------------------------------------------------------------------------
void Session::transactionsImported(QList<ImportedTransaction> transactions)
{
	importedTransactions = transactions;
	transactionsModel->setTransactions(transactions);
	assignTransactions();
}

//------------------------------------------------------------------------------
void Session::assignTransactions()
{
	/*
	for (int i=0; i<importedTransactions.size(); ++i)
	{
		ImportedTransaction trn = importedTransactions.at(i);
		qDebug() << trn.date() << trn.amount().toString() << trn.payee() << trn.memo()
			<< trn.withdrawalAccount() << trn.depositAccount();
	}
	*/
	emit showMessage("Transactions assigned");
	emit showProgress(66, 100);
}

//------------------------------------------------------------------------------
void Session::calculateBalances()
{
	emit showProgress(0, 0);
	emit showProgress(100, 100);
}

//------------------------------------------------------------------------------
void Session::showAnalysisSummary()
{ }

//------------------------------------------------------------------------------
void Session::showEstimateProgress()
{
	if (budget && estimateDisplay)
	{
		estimateDisplay->showEstimateProgress();
		setCurrentWidget(estimateDisplay);
	}
}

//------------------------------------------------------------------------------
void Session::showEstimateImpact()
{
	if (budget && estimateDisplay)
	{
		estimateDisplay->showBalanceImpact();
		setCurrentWidget(estimateDisplay);
	}
}

//------------------------------------------------------------------------------
void Session::showImportedTransactions()
{
	if (budget && transactionsList)
	{
		setCurrentWidget(transactionsList);
	}
}

//------------------------------------------------------------------------------
QString Session::sessionName() const
{
	QString location = tr("Unsaved");
	if ( ! budgetSource.isNull())
	{
		location = budgetSource->location();

		// If location is a file, truncate to just the file name
		QFileInfo info(location);
		if (info.exists())
		{
			location = info.fileName();
		}
	}

	return tr("%1 (%2)").arg(budgetName()).arg(location);
}

//------------------------------------------------------------------------------
QString Session::budgetName() const
{
	if (budget)
		return budget->name();
	return tr("No Budget");
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetSource> Session::currentBudgetSource() const
{
	return budgetSource;
}

//------------------------------------------------------------------------------
bool Session::hasUndoableActions() const
{
	return undoStack->canUndo();
}

//------------------------------------------------------------------------------
bool Session::hasRedoableActions() const
{
	return undoStack->canRedo();
}

//------------------------------------------------------------------------------
void Session::undo()
{
	undoStack->undo();
}

//------------------------------------------------------------------------------
void Session::redo()
{
	undoStack->redo();
}

}

