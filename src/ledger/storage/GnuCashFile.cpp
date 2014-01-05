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
#include "settings.hpp"
#include "ledger/storage/GnuCashFile.hpp"
#include "ledger/storage/GnuCashReader.hpp"

namespace ub {

//------------------------------------------------------------------------------
GnuCashFile::GnuCashFile(const QString& fileName)
	: fileName(fileName), isImporting(false), fileChanges(0)
{
	// Make sure we can pass these between threads via signals/slots
	qRegisterMetaType<ImportedTransactionSource::Result>("ImportedTransactionSource::Result");
	qRegisterMetaType<QList<ImportedTransaction> >("QList<ImportedTransaction>");

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

	// Watch file for changes, if auto-re-import enabled
	QSettings settings;
	if (settings.value(import::AutoReImport).toBool())
	{
		watcher.addPath(fileName);
		connect(&watcher, SIGNAL(fileChanged(QString)),
			this, SLOT(fileChanged(QString)));
	}
}

//------------------------------------------------------------------------------
void GnuCashFile::fileChanged(const QString& fileName)
{
	// Need to re-submit the file to the watcher so we can continue to
	// receive on-change signals
	watcher.removePath(fileName);
	watcher.addPath(fileName);

	// We receive this signal twice--but the first time, the file does not have
	// updated data yet. So when the second signal comes through, it does not
	// trigger an import since the first import is still in progress. Thus, we
	// ignore the first signal, and every other after that.
	if (++fileChanges % 2 == 0)
	{
		qDebug() << "GnuCash file" << fileName
			<< "has changed externally, signalling new data available...";
		emit newDataAvailable();
	}
}

//------------------------------------------------------------------------------
GnuCashFile::~GnuCashFile()
{
	cancel();
	thread.quit();
	thread.wait();
}

//------------------------------------------------------------------------------
QString GnuCashFile::name() const
{
	return fileName;
}

//------------------------------------------------------------------------------
QString GnuCashFile::location() const
{
	return fileName;
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

//------------------------------------------------------------------------------
bool GnuCashFile::isGnuCashFile(QFile& file)
{
	// Try simple file extension check
	if (file.fileName().endsWith("gnucash"))
		return true;

	// Check if a gzip file
	char twoBytes[2];
	if (file.peek(twoBytes, sizeof(twoBytes)) == sizeof(twoBytes))
	{
		char byte1 = 0x1f;
		char byte2 = 0x8b;

		// GZip files contain 0x1f8b as first two bytes
		if ((twoBytes[0] == byte1) && (twoBytes[1] == byte2))
			return true;
	}

	// Check if an XML file
	char twoLines[50];
	if (file.peek(twoLines, sizeof(twoLines)) == sizeof(twoLines))
	{
		if (QString(twoLines).contains("gnc"))
			return true;
	}

	return false;
}

}

