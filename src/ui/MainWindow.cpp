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
// This implementation file defines all MainWindow methods that are not:
// - menu/toolbar action slots (MainWindow_actions.cpp)
// - menu or toolbar methods (MainWindow_menus.cpp)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const int MainWindow::MAX_RECENT_BUDGET_FILES = 5;
const QString MainWindow::RECENT_BUDGET_FILES = "RecentBudgetFiles";
const QString MainWindow::LAST_USED_BUDGET_DIR = "LastUsedBudgetDir";
const QString MainWindow::MAIN_WINDOW_SIZE = "MainWindowSize";
const QString MainWindow::MAIN_WINDOW_STATE = "MainWindowState";

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
	QMdiSubWindow* window = mdiArea->addSubWindow(session);
	window->showMaximized();
	connect(session, SIGNAL(budgetNameChanged(QString)),
		window, SLOT(setWindowTitle(QString)));
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
	QSize size = settings.value(MAIN_WINDOW_SIZE, QSize(600, 500)).toSize();
	QByteArray state = settings.value(MAIN_WINDOW_STATE, QByteArray()).toByteArray();
	restoreState(state);
	resize(size);
}

//--------------------------------------------------------------------------
void MainWindow::writeSettings()
{
	QSettings settings;
	settings.setValue(MAIN_WINDOW_SIZE, size());
	settings.setValue(MAIN_WINDOW_STATE, saveState());
}

//------------------------------------------------------------------------------
void MainWindow::recordRecentBudget(const QString& file)
{
	QSettings settings;
	QStringList files = settings.value(RECENT_BUDGET_FILES).toStringList();
	files.removeAll(file);
	files.prepend(file);
	while (files.size() > MAX_RECENT_BUDGET_FILES)
	{
		files.removeLast();
	}
	settings.setValue(RECENT_BUDGET_FILES, files);
}

//------------------------------------------------------------------------------
void MainWindow::openBudget(const QString fileName)
{
	QSettings settings;
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
			// Record the directory this file is in so that the next open is
			// in the same directory
			QString fileDir = QFileInfo(fileName).canonicalPath();
			settings.setValue(LAST_USED_BUDGET_DIR, fileDir);
			recordRecentBudget(fileName);

			statusBar()->showMessage(QString("%1 opened").arg(fileName), 2000);
			session->show();
		}
		else
		{
			session->close();
		}
	}
}

}

