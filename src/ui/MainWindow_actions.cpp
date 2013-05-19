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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/MainWindow.hpp"
#include "ui/Session.hpp"
#include "ui/wizard/BudgetSourceWizard.hpp"

namespace ub
{

//------------------------------------------------------------------------------
// This implementation file defines all MainWindow slots associated with menu
// and toolbar actions.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void MainWindow::notImpl()
{
	QMessageBox::warning(this, tr("Unimplemented Feature"),
		tr("The requested feature has yet to be implemented."));
}

//--------------------------------------------------------------------------
void MainWindow::about()
{
	QMessageBox::about(this, tr("About"),
		tr(QString("<dl>"
			"<dt>Title</dt><dd>%1</dd>"
			"<dt>Version</dt><dd>%2</dd>"
			"<dt>Copyright</dt><dd>Copyright (C) 2013</dd>"
			"<dt>License</dt><dd>Apache License 2.0</dd>"
			"<dt>Authors</dt><dd>Kyle Treubig</dd>"
			"<dt>URL</dt><dd><a href=\"http://%3\">%3</a></dd>"
			"</dl>")
			.arg(qApp->applicationName())
			.arg(qApp->applicationVersion())
			.arg(qApp->organizationDomain())
			.toUtf8()
		));
}

//------------------------------------------------------------------------------
void MainWindow::newBudget()
{
	Session* session = createSession();
	session->newBudget();
	session->show();
}

//------------------------------------------------------------------------------
void MainWindow::openBudget()
{
	openBudget(BudgetSourceWizard::promptForBudgetToOpen(this));
}

//------------------------------------------------------------------------------
void MainWindow::openRecentBudget()
{
	// Get the file name from the triggering action
	QAction* action = qobject_cast<QAction*>(sender());
	if (action)
	{
		openBudget(BudgetSourceWizard::promptToReOpen(this,
			action->data().toString()));
	}
}

//------------------------------------------------------------------------------
void MainWindow::openBudget(QSharedPointer<BudgetSource> source)
{
	if ( ! source.isNull())
	{
		// If this budget source is already open, don't re-open, just bring
		// that window to the front
		QMdiSubWindow* existing = findSession(source);
		if (existing)
		{
			mdiArea->setActiveSubWindow(existing);
			return;
		}

		Session* session = createSession();
		if (session->openBudget(source))
		{
			recordRecentBudget(source->location());
			showStatusMessage(tr("%1 opened")
				.arg(session->currentBudgetSource()->location()));
			session->show();
		}
		else
		{
			QMdiSubWindow* sub = findSession(source);
			if (sub)
			{
				sub->close();
			}
		}
	}
}

//------------------------------------------------------------------------------
void MainWindow::saveBudget()
{
	Session* session = activeSession();
	if (session && session->save())
	{
		showStatusMessage(tr("%1 saved to %2").arg(session->budgetName())
			.arg(session->currentBudgetSource()->location()));
	}
}

//------------------------------------------------------------------------------
void MainWindow::saveBudgetAs()
{
	Session* session = activeSession();
	if (session && session->saveAs())
	{
		QString location = session->currentBudgetSource()->location();
		recordRecentBudget(location);
		showStatusMessage(tr("%1 saved to %2").arg(session->budgetName())
			.arg(location));
	}
}

//------------------------------------------------------------------------------
void MainWindow::saveBudgetAsTemplate()
{
	Session* session = activeSession();
	if (session && session->saveAsTemplate())
	{
		showStatusMessage(tr("Template budget updated with %1")
			.arg(session->budgetName()));
	}
}

//------------------------------------------------------------------------------
void MainWindow::editBudget()
{
	if (activeSession())
	{
		activeSession()->editBudget();
	}
}

//------------------------------------------------------------------------------
void MainWindow::editEstimates()
{
	if (activeSession())
	{
		activeSession()->editEstimates();
	}
}

//------------------------------------------------------------------------------
void MainWindow::editAssignmentRules()
{
	if (activeSession())
	{
		activeSession()->editAssignmentRules();
	}
}

//------------------------------------------------------------------------------
void MainWindow::importTransactions()
{
	if (activeSession())
	{
		activeSession()->importTransactions();
	}
}

//------------------------------------------------------------------------------
void MainWindow::importTransactionsFrom()
{
	if (activeSession())
	{
		activeSession()->importTransactionsFrom();
	}
}

//------------------------------------------------------------------------------
void MainWindow::assignTransactions()
{
	if (activeSession())
	{
		activeSession()->assignTransactions();
	}
}

//------------------------------------------------------------------------------
void MainWindow::calculateBalances()
{
	if (activeSession())
	{
		activeSession()->calculateBalances();
	}
}

//------------------------------------------------------------------------------
void MainWindow::showAnalysisSummary()
{
	if (activeSession())
	{
		activeSession()->showAnalysisSummary();
	}
}

//------------------------------------------------------------------------------
void MainWindow::showEstimateProgress()
{
	if (activeSession())
	{
		activeSession()->showEstimateProgress();
	}
}

//------------------------------------------------------------------------------
void MainWindow::showEstimateImpact()
{
	if (activeSession())
	{
		activeSession()->showEstimateImpact();
	}
}

//------------------------------------------------------------------------------
void MainWindow::showImportedTransactions()
{
	if (activeSession())
	{
		activeSession()->showImportedTransactions();
	}
}

}

