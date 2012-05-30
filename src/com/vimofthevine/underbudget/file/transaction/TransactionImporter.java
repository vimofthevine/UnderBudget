/*
 * Copyright 2012 Kyle Treubig
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

package com.vimofthevine.underbudget.file.transaction;

import java.io.InputStream;
import java.util.List;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.progress.TaskProgress;

/**
 * Interface for transaction importers.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionImporter {

	/**
	 * Parses the given input stream, extracting the list of
	 * transactions defined in the stream
	 * 
	 * @param stream   input stream from which to read
	 * @param period   budgeting period to use for restricting
	 *                  the transactions that are imported
	 * @param progress parse task progress
	 * @throws TransactionImportException if any error occurs
	 */
	public List<Transaction> read(InputStream stream, BudgetingPeriod period,
		TaskProgress progress) throws TransactionImportException;
	
}
