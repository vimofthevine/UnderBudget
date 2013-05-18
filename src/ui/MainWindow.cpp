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
MainWindow::MainWindow()
{
	// Set up window widgets
	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
		this, SLOT(updateMenus()));
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(setActiveSubWindow(QWidget*)));

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	updateMenus();

	readSettings();

	setWindowTitle(qApp->applicationName());
	setWindowIcon(QIcon(":/logo"));
	setUnifiedTitleAndToolBarOnMac(true);
}

//------------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
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

//------------------------------------------------------------------------------
void MainWindow::notImpl()
{
	QMessageBox::warning(this, tr("Unimplemented Feature"),
		tr("The requested feature has yet to be implemented."));
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
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Budget File"),
		"",
		tr("Budgets (*.ub);;XML files (*.xml);;All (*)"));
	if ( ! fileName.isEmpty())
	{
		QMdiSubWindow* existing = findSession(fileName);
		if (existing)
		{
			mdiArea->setActiveSubWindow(existing);
			return;
		}

		Session* session = createSession();
		if (session->openBudgetFile(fileName))
		{
			statusBar()->showMessage(QString("%1 opened").arg(fileName), 2000);
			session->show();
		}
		else
		{
			session->close();
		}
	}
}

//------------------------------------------------------------------------------
Session* MainWindow::activeSession() const
{
	if (QMdiSubWindow* activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<Session*>(activeSubWindow->widget());
	return 0;
}

//------------------------------------------------------------------------------
Session* MainWindow::createSession()
{
	Session* session = new Session;
	mdiArea->addSubWindow(session);
	return session;
}

//------------------------------------------------------------------------------
QMdiSubWindow* MainWindow::findSession(const QString& file) const
{
	QString filePath = QFileInfo(file).canonicalFilePath();

	foreach (QMdiSubWindow* window, mdiArea->subWindowList())
	{
		Session* session = qobject_cast<Session*>(window->widget());
		if (session->currentFileName() == filePath)
			return window;
	}

	return 0;
}

//------------------------------------------------------------------------------
void MainWindow::setActiveSubWindow(QWidget* window)
{
	if ( ! window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
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

