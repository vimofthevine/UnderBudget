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

#ifndef IMPORTEDTRANSACTIONSOURCE_HPP
#define IMPORTEDTRANSACTIONSOURCE_HPP

// Qt include(s)
#include <QList>
#include <QObject>
#include <QSharedPointer>

// UnderBudget include(s)
#include "ledger/ImportedTransaction.hpp"

namespace ub {

/**
 * Abstract imported transaction source class. Concrete implementations
 * may read transactions from files, databases, network locations, etc.
 *
 * It is assumed that the actual import operation will take place in a
 * separate thread. Thus, to acquire the imported transactions or check
 * the status of the import, use the appropriate signals emitted by an
 * instance of this class.
 *
 * @ingroup ledger_storage
 */
class ImportedTransactionSource : public QObject
{
	Q_OBJECT

public:
	/**
	 * Transaction import result enumeration.
	 */
	enum Result {
		Cancelled,
		Complete,
		FailedWithError
	};

	/** Virtual destructor */
	virtual ~ImportedTransactionSource();

	/**
	 * Returns the name or a description of this imported
	 * transaction source.
	 *
	 * @return imported transaction source name or description
	 */
	virtual QString name() const = 0;

	/**
	 * Returns a description of this source's location. It could be a file name,
	 * a database name, a remote network location, etc.
	 *
	 * @return source location
	 */
	virtual QString location() const = 0;

public slots:
	/**
	 * Initiates an import of transactions.
	 *
	 * This will create a new thread in which to perform the import
	 * operation. Thus, this method will return immediately.
	 *
	 * When the import has finished, the `finished` signal will be
	 * emitted. If successful, the `imported` signal will also be
	 * emitted.
	 *
	 * @return `false` if an import is already in progress
	 */
	virtual bool import() = 0;

	/**
	 * Initiates an import of transactions within the given date range.
	 *
	 * Only transactions that occurred on or between the given start and
	 * end dates will be imported. Those occurring before the start date
	 * or after the end date will be ignored.
	 *
	 * This will create a new thread in which to perform the import
	 * operation. Thus, this method will return immediately.
	 *
	 * When the import has finished, the `finished` signal will be
	 * emitted. If successful, the `imported` signal will also be
	 * emitted.
	 *
	 * @param[in] start start date of import range
	 * @param[in] end   end date of import range
	 * @return `false` if an import is already in progress
	 */
	virtual bool import(const QDate& start, const QDate& end) = 0;

	/**
	 * Cancels the current import operation, if one is in progress.
	 */
	virtual void cancel() = 0;

signals:
	/**
	 * Emitted when an import operation commences.
	 */
	void started();

	/**
	 * Emitted when an import operation is completed.
	 *
	 * When a failure has occurred, `message` will contain the error
	 * message as detected. For a successful import, `message` will
	 * indicate the number of transactions that were imported.
	 *
	 * @param result  import result
	 * @param message import message
	 */
	void finished(ImportedTransactionSource::Result result,
		const QString& message);

	/**
	 * If emitted, indicates the current progress of the
	 * transaction import as a percentage (out of 100).
	 *
	 * @param percent import percent complete
	 */
	void progress(int percent);

	/**
	 * Emitted when the import has finished and has successfully imported
	 * a list of transactions. If the import failed or was cancelled, this
	 * signal is not emitted.
	 *
	 * @param transactions imported transactions
	 */
	void imported(QList<ImportedTransaction> transactions);

	/**
	 * Emitted when new data is available for import.
	 */
	void newDataAvailable();
};

}

// Make Result known to QMetaType
Q_DECLARE_METATYPE(ub::ImportedTransactionSource::Result);

#endif //IMPORTEDTRANSACTIONSOURCE_HPP
