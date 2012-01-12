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

package com.vimofthevine.underbudget.transactions.importer.parsers;

import java.io.InputStream;
import java.util.List;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.importer.ImportFileException;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Interface for import file parsers. This facilitates
 * importing from arbitrary financial record sources
 * (GnuCash, QuickBooks, etc.)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface ImportFileParser {

	/**
	 * Returns a reference to the task progress updater
	 * for the parser. This allows for progress listeners
	 * to register with the parser for progress updates.
	 * 
	 * @return task progress updater for the parser
	 */
	public TaskProgress getProgress();
	
	/**
	 * Parses the given input stream as an import file,
	 * or a source of transaction records
	 * 
	 * @param stream input stream from which to read
	 * @param period budgeting period to use for restricting
	 *               the transactions that are imported
	 * @throws ImportFileException if any errors occur
	 */
	public void parse(InputStream stream, BudgetingPeriod period) throws ImportFileException;
	
	/**
	 * Returns the list of transaction records extracted from the file
	 * 
	 * @param parsed transaction list
	 */
	public List<Transaction> getTransactions();
	
}
