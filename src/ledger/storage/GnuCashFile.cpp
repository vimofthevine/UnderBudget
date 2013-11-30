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
#include <QtCore>

// UnderBudget include(s)
#include "ledger/storage/GnuCashFile.hpp"
#include "ledger/storage/GnuCashReader.hpp"

namespace ub {

//------------------------------------------------------------------------------
GnuCashFile::GnuCashFile(const QString& fileName)
	: isImporting(false)
{
	reader = new GnuCashReader(fileName);
	reader->moveToThread(&thread);

	// Make sure the reader is deleted when the thread terminates
	connect(&thread, SIGNAL(finished()), reader, SLOT(deleteLater()));

	// Forward results from the reader thread back to this thread
	connect(reader, SIGNAL(started()), this, SIGNAL(started()));
	connect(reader, SIGNAL(finished(ImportedTransactionSource::Result, QString)),
		this, SIGNAL(finished(ImportedTransactionSource::Result, QString)));
	connect(reader, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
	connect(reader, SIGNAL(imported(QList<ImportedTransaction>)),
		this, SIGNAL(imported(QList<ImportedTransaction>)));

	// Record importing state of the reader thread
	connect(reader, SIGNAL(started()), this, SLOT(importingStarted()));
	connect(reader, SIGNAL(finished(ImportedTransactionSource::Result, QString)),
		this, SLOT(importingFinished()));

	// Start event loop in reader thread
	thread.start();
}

//------------------------------------------------------------------------------
GnuCashFile::~GnuCashFile()
{
	cancel();
	thread.quit();
	thread.wait();
}

//------------------------------------------------------------------------------
void GnuCashFile::importingStarted()
{
	isImporting = true;
}

//------------------------------------------------------------------------------
void GnuCashFile::importingFinished()
{
	isImporting = false;
}

//------------------------------------------------------------------------------
bool GnuCashFile::import()
{
	return import(QDate(), QDate());
}

//------------------------------------------------------------------------------
bool GnuCashFile::import(const QDate& start, const QDate& end)
{
	if ( ! isImporting)
	{
		QMetaObject::invokeMethod(reader, "import", Qt::QueuedConnection,
			Q_ARG(QDate, start), Q_ARG(QDate, end));
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
void GnuCashFile::cancel()
{
	QMetaObject::invokeMethod(reader, "cancel");
}

}

