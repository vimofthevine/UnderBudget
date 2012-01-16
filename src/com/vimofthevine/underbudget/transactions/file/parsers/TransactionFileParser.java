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

package com.vimofthevine.underbudget.transactions.file.parsers;

import java.io.InputStream;
import java.util.List;

import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.TransactionFileException;
import com.vimofthevine.underbudget.transactions.importer.parsers.ImportFileParser;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Interface for transaction file parsers. It is not expected
 * that there would be multiple runtime-swappable file parsers,
 * but rather that in case the file format changes in the format,
 * the parser can be easily swapped out.
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionFileParser extends ImportFileParser {
	
	/**
	 * Returns a reference to the task progress updater
	 * for the parser. This allows for progress listeners
	 * to register with the parser for progress updates.
	 * 
	 * @return task progress updater for the parser
	 */
	public TaskProgress getProgress();
	
	/**
	 * Parses the given input stream as a transaction file
	 * 
	 * @param stream input stream from which to read
	 * @throws TransactionFileException if there are any errors with the file
	 */
	public void parse(InputStream stream) throws TransactionFileException;
	
	/**
	 * Returns the list of transaction specified in the file
	 * 
	 * @return list of parsed transactions
	 */
	public List<Transaction> getTransactions();

}
