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
#include "ui/SdiWindow.hpp"
#include "ui/Session.hpp"

namespace ub
{

//------------------------------------------------------------------------------
SdiWindow::SdiWindow(QSharedPointer<BudgetSource> source)
	: MainWindow(), session(0)
{
	// Clean up menus
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeWindow()));
	connect(closeAllAction, SIGNAL(triggered()), this, SLOT(closeAllWindows()));
	updateMenus();

	// Open budget right away, if one specified
	if ( ! source.isNull())
	{
		openBudget(source);
	}
}

//------------------------------------------------------------------------------
SdiWindow::SdiWindow(bool)
	: MainWindow(), session(0)
{
	// Clean up menus
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeWindow()));
	connect(closeAllAction, SIGNAL(triggered()), this, SLOT(closeAllWindows()));
	fileMenu->removeAction(closeAllAction);
	updateMenus();

	// Create new budget right away
	newBudget();
}

//------------------------------------------------------------------------------
void SdiWindow::closeEvent(QCloseEvent* event)
{
	// If the user cancelled the close due to pending changes
	if (session && ! session->close())
	{
		event->ignore();
	}
	else
	{
		MainWindow::closeEvent(event);
	}
}

//------------------------------------------------------------------------------
void SdiWindow::takeFocus()
{
	show();
	raise();
	activateWindow();
}

//------------------------------------------------------------------------------
Session* SdiWindow::activeSession() const
{
	return session;
}

//------------------------------------------------------------------------------
void SdiWindow::createSession()
{
	session = new Session;
	connect(session, SIGNAL(undoAvailable(bool)),
		undoAction, SLOT(setEnabled(bool)));
	connect(session, SIGNAL(redoAvailable(bool)),
		redoAction, SLOT(setEnabled(bool)));
	connect(session, SIGNAL(showMessage(QString)),
		this, SLOT(showStatusMessage(QString)));
	connect(session, SIGNAL(showProgress(int, int)),
		this, SLOT(showProgress(int, int)));
	connect(session, SIGNAL(titleChanged(QString)),
		this, SLOT(setWindowTitle(QString)));
	connect(session, SIGNAL(budgetModified(bool)),
		this, SLOT(setWindowModified(bool)));
}

//------------------------------------------------------------------------------
void SdiWindow::newBudget()
{
	if ( ! session)
	{
		createSession();
		session->newBudget();
		setCentralWidget(session);
		updateMenus();
	}
	else
	{
		SdiWindow* other = new SdiWindow(true);
		other->move(x() + 40, y() + 40);
		other->show();
	}
}

//------------------------------------------------------------------------------
void SdiWindow::openBudget(QSharedPointer<BudgetSource> source)
{
	if ( ! source.isNull())
	{
		SdiWindow* existing = findMainWindow(source);
		// If already open in another window
		if (existing)
		{
			existing->takeFocus();
			return;
		}
		// If no budget open yet in this window
		else if ( ! session)
		{
			createSession();
			if (session->openBudget(source))
			{
				recordRecentBudget(source->location());
				setCentralWidget(session);
				updateMenus();
			}
			else
			{
				session = 0;
			}
		}
		// Else open in a new window
		else
		{
			SdiWindow* other = new SdiWindow(source);
			other->move(x() + 40, y() + 40);
			other->show();
		}
	}
}

//------------------------------------------------------------------------------
SdiWindow* SdiWindow::findMainWindow(const QSharedPointer<BudgetSource>& source) const
{
	QString location = source->location();

	foreach (QWidget* widget, qApp->topLevelWidgets())
	{
		SdiWindow* window = qobject_cast<SdiWindow*>(widget);
		if (window)
		{
			Session* otherSession = window->session;
			if (otherSession)
			{
				QSharedPointer<BudgetSource> src = otherSession->currentBudgetSource();
				if ( ! src.isNull() && src->location() == location)
					return window;
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
void SdiWindow::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAction);
	windowMenu->addAction(closeAllAction);

	QList<QWidget*> windows = qApp->topLevelWidgets();
	if ( ! windows.isEmpty())
	{
		windowMenu->addSeparator();
	}

	int num = 1;
	for (int i=0; i<windows.size(); ++i)
	{
		SdiWindow* window = qobject_cast<SdiWindow*>(windows.at(i));
		if (window)
		{
			Session* session = window->session;
			QString sessionName = session ? session->sessionName() : "<untitled>";

			QString text;
			if (num < 9)
			{
				text = tr("&%1 %2").arg(num++).arg(sessionName);
			}
			else
			{
				text = tr("%1 %2").arg(num++).arg(sessionName);
			}

			QAction* action = windowMenu->addAction(text);
			action->setCheckable(true);
			action->setChecked(this == window);
			connect(action, SIGNAL(triggered()), window, SLOT(takeFocus()));
		}
	}
}

//------------------------------------------------------------------------------
bool onlyOneWindowOpen()
{
	int count = 0;

	QList<QWidget*> windows = qApp->topLevelWidgets();
	for (int i=0; i<windows.size(); ++i)
	{
		if (qobject_cast<SdiWindow*>(windows.at(i)))
			count++;
	}

	return count == 1;
}

//------------------------------------------------------------------------------
bool SdiWindow::closeWindow(bool keepOpen)
{
	if (keepOpen || onlyOneWindowOpen())
	{
		if (session && session->close())
		{
			session = 0;
			setCentralWidget(0);
			setWindowTitle(qApp->applicationName());
			updateMenus();
		}
		return false;
	}
	else
	{
		close();
		return true;
	}
}

//------------------------------------------------------------------------------
void SdiWindow::closeAllWindows()
{
	QList<QWidget*> windows = qApp->topLevelWidgets();
	int numOpenWindows = windows.size();
	for (int i=windows.size()-1; i>=0; --i)
	{
		SdiWindow* window = qobject_cast<SdiWindow*>(windows.at(i));
		if (window)
		{
			// Keep this window open if its the last one and no others are open
			bool windowClosed = window->closeWindow((numOpenWindows == 1) && (i == 0));
			if (windowClosed)
				numOpenWindows--;
		}
		else
			numOpenWindows--;
	}
}

}

