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
#include "ui/MdiWindow.hpp"
#include "ui/Session.hpp"

namespace ub
{

//------------------------------------------------------------------------------
MdiWindow::MdiWindow()
	: MainWindow()
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

	// Clean up menus
	connect(closeAction, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
	connect(closeAllAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
	connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
	connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
}

//--------------------------------------------------------------------------
void MdiWindow::closeEvent(QCloseEvent* event)
{
	mdiArea->closeAllSubWindows();
	if (mdiArea->currentSubWindow())
	{
		event->ignore();
	}
	else
	{
		MainWindow::closeEvent(event);
	}
}

//------------------------------------------------------------------------------
Session* MdiWindow::activeSession() const
{
	if (QMdiSubWindow* activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<Session*>(activeSubWindow->widget());
	return 0;
}

//------------------------------------------------------------------------------
Session* MdiWindow::createSession()
{
	Session* session = new Session;
	QMdiSubWindow* window = mdiArea->addSubWindow(session);
	connect(session, SIGNAL(undoAvailable(bool)),
		undoAction, SLOT(setEnabled(bool)));
	connect(session, SIGNAL(redoAvailable(bool)),
		redoAction, SLOT(setEnabled(bool)));
	connect(session, SIGNAL(showMessage(QString)),
		this, SLOT(showStatusMessage(QString)));
	connect(session, SIGNAL(showProgress(int, int)),
		this, SLOT(showProgress(int, int)));
	window->showMaximized();
	return session;
}

//------------------------------------------------------------------------------
void MdiWindow::newBudget()
{
	Session* session = createSession();
	session->newBudget();
	session->show();
}

//------------------------------------------------------------------------------
void MdiWindow::openBudget(QSharedPointer<BudgetSource> source)
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
QMdiSubWindow* MdiWindow::findSession(const QSharedPointer<BudgetSource>& source) const
{
	QString location = source->location();

	foreach (QMdiSubWindow* window, mdiArea->subWindowList())
	{
		Session* session = qobject_cast<Session*>(window->widget());
		QSharedPointer<BudgetSource> src = session->currentBudgetSource();
		if ( ! src.isNull() && src->location() == location)
			return window;
	}

	return 0;
}

//------------------------------------------------------------------------------
void MdiWindow::setActiveSubWindow(QWidget* window)
{
	if ( ! window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

//------------------------------------------------------------------------------
void MdiWindow::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAction);
	windowMenu->addAction(closeAllAction);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);

	QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
	if ( ! windows.isEmpty())
	{
		windowMenu->addSeparator();
	}

	for (int i=0; i<windows.size(); ++i)
	{
		Session* session = qobject_cast<Session*>(windows.at(i)->widget());

		QString text;
		if (i < 9)
		{
			text = tr("&%1 %2").arg(i+1)
				.arg(session->sessionName());
		}
		else
		{
			text = tr("%1 %2").arg(i+1)
				.arg(session->sessionName());
		}

		QAction* action = windowMenu->addAction(text);
		action->setCheckable(true);
		action->setChecked(session == activeSession());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

}

