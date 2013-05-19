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
#include "ui/wizard/BudgetFileWizard.hpp"

namespace ub {

//------------------------------------------------------------------------------
const QString BudgetFileWizard::LAST_USED_BUDGET_DIR = "LastUsedBudgetDir";

//------------------------------------------------------------------------------
QString BudgetFileWizard::promptForFileToOpen(QWidget* parent)
{
	QSettings settings;
	QString fileName = QFileDialog::getOpenFileName(parent,
		QObject::tr("Open Budget File"),
		settings.value(LAST_USED_BUDGET_DIR).toString(),
		QObject::tr("Budgets (*.ub);;XML files (*.xml);;All (*)"));

	if ( ! fileName.isEmpty())
	{
		// Record the directory this file is in so that the next open is
		// in the same directory
		QString fileDir = QFileInfo(fileName).canonicalPath();
		settings.setValue(LAST_USED_BUDGET_DIR, fileDir);
	}

	return fileName;
}

//------------------------------------------------------------------------------
QString BudgetFileWizard::promptForFileToSave(QWidget* parent,
	const QString& existing)
{
	QString dir = existing;
	if (existing.isEmpty())
	{
		QSettings settings;
		dir = settings.value(LAST_USED_BUDGET_DIR).toString();
	}

	QString fileName = QFileDialog::getSaveFileName(parent,
		QObject::tr("Save Budget File"),
		dir, QObject::tr("Budgets (*.ub);;XML files (*.xml);;All (*)"));

	if ( ! fileName.isEmpty())
	{
		// Record the directory this file is in so that the next open is
		// in the same directory
		QFileInfo info(fileName);
		QString fileDir = info.exists()
			? info.canonicalPath()
			: info.absolutePath();
		QSettings settings;
		settings.setValue(LAST_USED_BUDGET_DIR, fileDir);
	}

	return fileName;
}

}

