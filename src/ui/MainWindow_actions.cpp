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
	session->newBudgetFile();
	session->show();
}

//------------------------------------------------------------------------------
void MainWindow::openBudget()
{
	QSettings settings;
	openBudget(
		QFileDialog::getOpenFileName(this, tr("Open Budget File"),
			settings.value(LAST_USED_BUDGET_DIR).toString(),
			tr("Budgets (*.ub);;XML files (*.xml);;All (*)"))
	);
}

//------------------------------------------------------------------------------
void MainWindow::openRecentBudget()
{
	// Get the file name from the triggering action
	QAction* action = qobject_cast<QAction*>(sender());
	if (action)
	{
		openBudget(action->data().toString());
	}
}

}

