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

package com.vimofthevine.underbudget.transactions.file.writers;

import java.io.OutputStream;
import java.util.List;

import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.TransactionFileException;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Interface for transaction file writers. It is not expected
 * that there would be multiple runtime-swappable file writers,
 * but rather that in case the file format changes in the future,
 * the writer can be easily swapped out.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionFileWriter {
	
	/**
	 * Returns a reference to the task progress updater
	 * for the writer. This allows for progress listeners
	 * to register with the parser for progress updates.
	 * 
	 * @return task progress updater for the parser
	 */
	public TaskProgress getProgress();
	
	/**
	 * Write the transaction list to the given output stream
	 * 
	 * @param stream       output stream on which to write
	 * @param transactions transaction list to be written
	 * @throws TransactionFileException if any error occurs
	 */
	public void write(OutputStream stream, List<Transaction> transactions)
	throws TransactionFileException;

}
