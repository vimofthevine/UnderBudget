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
const QString MainWindow::MAIN_WINDOW_SIZE = "MainWindowSize";
const QString MainWindow::MAIN_WINDOW_STATE = "MainWindowState";

//------------------------------------------------------------------------------
MainWindow::MainWindow()
{
	// Set up window widgets
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	readSettings();

	setWindowTitle(qApp->applicationName());
	setWindowIcon(QIcon(":/logo"));
	setUnifiedTitleAndToolBarOnMac(true);
	setAttribute(Qt::WA_DeleteOnClose, true);
}

//------------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));

	progressBar = new QProgressBar;
	progressBar->setMaximumSize(170, 19);
	progressBar->setTextVisible(false);
	progressBar->setEnabled(false);
	progressBar->setMinimum(0);
	progressBar->setMaximum(100);
	progressBar->setValue(100);
	progressBar->setVisible(false);
	statusBar()->addPermanentWidget(progressBar);
}

//------------------------------------------------------------------------------
void MainWindow::showStatusMessage(const QString& message)
{
	statusBar()->showMessage(message, 2000);
}

//------------------------------------------------------------------------------
void MainWindow::showProgress(int value, int max)
{
	// Indefinite/busy indicator
	if (max == 0)
	{
		progressBar->setVisible(true);
	}
	else
	{
		// If finished (i.e., 100%)
		if (value == max && progressBar->isVisible())
		{
			progressBar->setVisible(false);
		}
		// Else in-progress (0 <= % < 100)
		else if (value != max && ! progressBar->isVisible())
		{
			progressBar->setVisible(true);
		}
	}

	progressBar->setMinimum(0);
	progressBar->setMaximum(max);
	progressBar->setValue(value);
}

//--------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent* event)
{
	writeSettings();
	QMainWindow::closeEvent(event);
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

}

