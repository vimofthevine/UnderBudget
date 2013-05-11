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

namespace ub
{

//------------------------------------------------------------------------------
MainWindow::MainWindow()
{
	// Set up window widgets
	setWindowTitle(qApp->applicationName());

	QLabel* label = new QLabel(qApp->applicationVersion());
	setCentralWidget(label);
}

//--------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent* event)
{
	int result = QMessageBox::warning(this, tr("Close?"),
		tr("Close UnderBudget?"), QMessageBox::Yes | QMessageBox::Cancel);

	if (result == QMessageBox::Yes)
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
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

//--------------------------------------------------------------------------
void MainWindow::readSettings()
{
	QSettings settings;
	QSize size = settings.value("MainWindowSize", QSize(600, 500)).toSize();
	QByteArray state = settings.value("MainWindowState", QByteArray()).toByteArray();
	restoreState(state);
	resize(size);
}

//--------------------------------------------------------------------------
void MainWindow::writeSettings()
{
	QSettings settings;
	settings.setValue("MainWindowSize", size());
	settings.setValue("MainWindowState", saveState());
}

}

