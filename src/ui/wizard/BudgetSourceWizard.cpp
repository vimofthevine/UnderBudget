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
#include "budget/storage/XmlBudgetFile.hpp"
#include "ui/wizard/BudgetSourceWizard.hpp"

namespace ub {

//------------------------------------------------------------------------------
const QString BudgetSourceWizard::LAST_USED_BUDGET_DIR = "LastUsedBudgetDir";

//------------------------------------------------------------------------------
QSharedPointer<BudgetSource> BudgetSourceWizard::promptToReOpen(QWidget* parent,
	const QString& location)
{
	// Right now only file-based budget sources are supported

	QSharedPointer<BudgetSource> source;
	QString fileName = location;

	if ( ! fileName.isEmpty())
	{
		recordLastUsedPath(fileName);
		source.reset(createForFile(fileName));

		if (source.isNull())
		{
			QMessageBox::warning(parent, QObject::tr("Unknown File Type"),
				QObject::tr("The file, %1, is an unsupported file type").arg(fileName));
		}
	}

	return source;
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetSource> BudgetSourceWizard::promptForBudgetToOpen(
	QWidget* parent)
{
	// Right now only file-based budget sources are supported

	QSettings settings;
	QString fileName = QFileDialog::getOpenFileName(parent,
		QObject::tr("Open Budget File"),
		settings.value(LAST_USED_BUDGET_DIR).toString(),
		QObject::tr("Budgets (*.budget *.xml);;All (*)"));

	QSharedPointer<BudgetSource> source;

	if ( ! fileName.isEmpty())
	{
		recordLastUsedPath(fileName);
		source.reset(createForFile(fileName));

		if (source.isNull())
		{
			QMessageBox::warning(parent, QObject::tr("Unknown File Type"),
				QObject::tr("The file, %1, is an unsupported file type").arg(fileName));
		}
	}

	return source;
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetSource> BudgetSourceWizard::promptForBudgetToSave(
	QWidget* parent, QSharedPointer<BudgetSource> existing)
{
	// Right now only file-based budget sources are supported

	// Try using the existing source's directory location.
	// This would need to be modified if non-file-based sources exist
	QString dir = existing.isNull() ? "" : existing->location();
	if (dir.isEmpty())
	{
		// Else use the last-used budget file directory
		QSettings settings;
		dir = settings.value(LAST_USED_BUDGET_DIR).toString();
	}

	QString fileName = QFileDialog::getSaveFileName(parent,
		QObject::tr("Save Budget File"),
		dir, QObject::tr("Budgets (*.budget *.xml);;All (*)"));

	QSharedPointer<BudgetSource> source;

	if ( ! fileName.isEmpty())
	{
		recordLastUsedPath(fileName);
		source.reset(createForFile(fileName));

		if (source.isNull())
		{
			QMessageBox::warning(parent, QObject::tr("Unknown File Type"),
				QObject::tr("The file, %1, is an unsupported file type").arg(fileName));
		}
	}

	return source;
}

//------------------------------------------------------------------------------
void BudgetSourceWizard::recordLastUsedPath(const QString& fileName)
{
	QFileInfo info(fileName);
	QString fileDir = info.exists()
		? info.canonicalPath()
		: info.absolutePath();
	QSettings settings;
	settings.setValue(LAST_USED_BUDGET_DIR, fileDir);
}

//------------------------------------------------------------------------------
BudgetSource* BudgetSourceWizard::createForFile(const QString& fileName)
{
	if (fileName.endsWith("budget") || fileName.endsWith("xml"))
		return new XmlBudgetFile(fileName);
	else
		return 0;
}

}

