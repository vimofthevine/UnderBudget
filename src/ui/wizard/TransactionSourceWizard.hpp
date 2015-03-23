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

#ifndef TRANSACTIONSOURCEWIZARD_HPP
#define TRANSACTIONSOURCEWIZARD_HPP

// Qt include(s)
#include <QSharedPointer>

// UnderBudget include(s)
#include "ledger/storage/ImportedTransactionSource.hpp"

// Forward declaration(s)
class QWidget;

namespace ub {

/**
 * Transaction source selection wizard.
 *
 * @ingroup ui_wizard
 */
class TransactionSourceWizard {
public:
	/**
	 * Prompts the user for all information necessary to re-open a
	 * transaction source from the specified location. If the described
	 * transaction source could not be determined, the returned pointer
	 * will be null.
	 *
	 * @param[in] parent   parent widget
	 * @param[in] location transaction source location description
	 * @return transaction source from which to import transactions
	 */
	static QSharedPointer<ImportedTransactionSource>
		promptToReImport(QWidget* parent, const QString& location);

	/**
	 * Prompts the user for all information necessary to open a transaction
	 * source. If the user cancels the operation or the selected
	 * source is invalid, the returned pointer will be null.
	 *
	 * @param[in] parent parent widget
	 * @return transaction source from which to import transactions
	 */
	static QSharedPointer<ImportedTransactionSource>
		promptForTransactionImport(QWidget* parent);

private:
	// Last-used transaction file directory settings key
	static const QString LAST_USED_TRANSACTION_DIR;

	/**
	 * Records the directory containing the specified file as the last-used
	 * transaction file directory. This allows the next selection operation
	 * to begin in the same directory.
	 *
	 * @param[in] fileName name of a transaction file
	 */
	static void recordLastUsedPath(const QString& fileName);

	/**
	 * Creates a transaction import source for the specified file name. The
	 * type of the transaction source created depends on the extension of the
	 * file name. If the file extension is unrecognized, a null pointer is
	 * returned.
	 *
	 * @param[in] fileName name of a transaction file
	 * @return transaction import source for the specified transaction file,
	 *         or a null pointer if the specified file name contains an
	 *         unrecognized extension
	 */
	static ImportedTransactionSource* createForImportFile(const QString& fileName);
};

}

#endif //TRANSACTIONSOURCEWIZARD_HPP
