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

package com.vimofthevine.underbudget.transactions.file;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.writers.TransactionFileWriter;
import com.vimofthevine.underbudget.transactions.file.writers.TransactionFileWriterFactory;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Representation of a transaction file as it
 * is stored on a file system
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFile {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TransactionFile.class.getName());
	
	/**
	 * Transaction list defined in the file
	 */
	protected List<Transaction> transactions;
	
	/**
	 * Transaction file path
	 */
	protected String path;
	
	/**
	 * Transaction file writer
	 */
	protected TransactionFileWriter writer;
	
	/**
	 * Constructor, specifying the path to
	 * the transaction file
	 * 
	 * @param path file path
	 */
	public TransactionFile(String path)
	{
		this.path = path;
		
		writer = TransactionFileWriterFactory.createWriter();
	}
	
	/**
	 * Returns a reference to the file writer progress
	 * 
	 * @return file writer task progress
	 */
	public TaskProgress getWriterProgress()
	{
		return writer.getProgress();
	}
	
	/**
	 * Updates the transaction list stored in this file
	 * 
	 * @param transactions new transaction list
	 */
	public void setTransactions(List<Transaction> transactions)
	{
		this.transactions = transactions;
	}
	
	/**
	 * Writes the transaction file
	 * 
	 * @throws TransactionFileException if any error occurs
	 */
	public void write() throws TransactionFileException
	{
		try
		{
			OutputStream stream = new FileOutputStream(path);
			writer.write(stream, transactions);
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error writing file", ioe);
			throw new TransactionFileException("Unable to save transaction file, " + path);
		}
	}

}
