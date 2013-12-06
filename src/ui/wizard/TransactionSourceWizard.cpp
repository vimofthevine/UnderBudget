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
#include "ledger/storage/GnuCashFile.hpp"
#include "ui/wizard/TransactionSourceWizard.hpp"

namespace ub {

//------------------------------------------------------------------------------
const QString TransactionSourceWizard::LAST_USED_TRANSACTION_DIR = "LastUsedTrnDir";

//------------------------------------------------------------------------------
QSharedPointer<ImportedTransactionSource>
	TransactionSourceWizard::promptForTransactionImport(QWidget* parent)
{
	// Right now only file-based transaction sources are supported

	QSettings settings;
	QString fileName = QFileDialog::getOpenFileName(parent,
		QObject::tr("Select Transaction Import File"),
		settings.value(LAST_USED_TRANSACTION_DIR).toString(),
		QObject::tr("GnuCash (*.gnucash);;All (*)"));

	QSharedPointer<ImportedTransactionSource> source;

	if ( ! fileName.isEmpty())
	{
		recordLastUsedPath(fileName);
		source.reset(createForImportFile(fileName));

		if (source.isNull())
		{
			QMessageBox::warning(parent, QObject::tr("Unknown File Type"),
				QObject::tr("The file, %1, is an unsupported file type").arg(fileName));
		}
	}

	return source;
}

//------------------------------------------------------------------------------
void TransactionSourceWizard::recordLastUsedPath(const QString& fileName)
{
	QFileInfo info(fileName);
	QString fileDir = info.exists()
		? info.canonicalPath()
		: info.absolutePath();
	QSettings settings;
	settings.setValue(LAST_USED_TRANSACTION_DIR, fileDir);
}

//------------------------------------------------------------------------------
ImportedTransactionSource*
TransactionSourceWizard::createForImportFile(const QString& fileName)
{
	if (fileName.endsWith("gnucash"))
		return new GnuCashFile(fileName);
	else
		return 0;
}

}

