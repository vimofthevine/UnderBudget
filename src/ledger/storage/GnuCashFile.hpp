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

#ifndef GNUCASHFILE_HPP
#define GNUCASHFILE_HPP

// Qt include(s)
#include <QThread>

// UnderBudget include(s)
#include "ledger/storage/ImportedTransactionSource.hpp"

namespace ub {

// Forward declaration(s)
class GnuCashReader;

/**
 * GnuCash file model.
 */
class GnuCashFile : public ImportedTransactionSource
{
	Q_OBJECT

public:
	/**
	 * Constructs a GnuCash file for the specified path.
	 *
	 * @param[in] fileName GnuCash file location
	 */
	GnuCashFile(const QString& fileName);

	/**
	 * Terminates the reader thread.
	 */
	~GnuCashFile();

	/**
	 * Returns the name of the GnuCash file.
	 *
	 * @return GnuCash file name
	 */
	QString name() const;

public slots:
	// Implemented virtual methods
	bool import();
	bool import(const QDate& start, const QDate& end);
	void cancel();

private slots:
	/**
	 * Records the state of the reader as importing.
	 */
	void importingStarted();

	/**
	 * Records the state of the reader as idle/not importing.
	 */
	void importingFinished();

private:
	/** GnuCash file name */
	QString fileName;
	/** Thread in which to perform read/import operations */
	QThread thread;
	/** GnuCash file reader */
	GnuCashReader* reader;
	/** Whether the reader is currently importing/reading */
	bool isImporting;
};

}

#endif //GNUCASHFILE_HPP
