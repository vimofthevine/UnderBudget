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

package com.vimofthevine.underbudget.budget.file;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileParser;
import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileParserFactory;
import com.vimofthevine.underbudget.budget.file.writers.BudgetFileWriter;
import com.vimofthevine.underbudget.budget.file.writers.BudgetFileWriterFactory;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Representation of a budget file as it is
 * stored on a file system
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFile {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetFile.class.getName());
	
	/**
	 * Budget defined in the file
	 */
	protected Budget budget;
	
	/**
	 * Budget file path
	 */
	protected String path;
	
	/**
	 * Budget file parser
	 */
	protected BudgetFileParser parser;
	
	/**
	 * Budget file writer
	 */
	protected BudgetFileWriter writer;
	
	/**
	 * Constructor, specifying the path
	 * to the budget file
	 * 
	 * @param path file path
	 */
	public BudgetFile(String path)
	{
		this.path = path;
		
		parser = BudgetFileParserFactory.createParser();
		writer = BudgetFileWriterFactory.createWriter();
	}
	
	/**
	 * Returns the budget defined in the file
	 * 
	 * @return budget
	 */
	public Budget getBudget()
	{
		return budget;
	}
	
	/**
	 * Returns a reference to the file parser progress
	 * 
	 * @return file parser task progress
	 */
	public TaskProgress getParserProgress()
	{
		return parser.getProgress();
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
	 * Parses the budget file, extracting the
	 * budget defined in the file
	 * 
	 * @throws BudgetFileException if any error occurs
	 */
	public void parse() throws BudgetFileException
	{
		try
		{
			parse(new FileInputStream(path));
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error opening file", ioe);
			throw new BudgetFileException("Unable to open budget file, " + path);
		}
	}
	
	/**
	 * Parses the budget file from the given input stream
	 * 
	 * @param stream input stream from which to read the budget
	 * @throws BudgetFileException if any error occurs
	 */
	protected void parse(InputStream stream) throws BudgetFileException
	{
		try
		{
			parser.parse(stream);
			budget = parser.getBudget();
			stream.close();
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error reading from stream", ioe);
			throw new BudgetFileException("Unable to read budget from stream, " + stream);
		}
	}
	
	/**
	 * Writes the budget to the budget file
	 * 
	 * @throws BudgetFileException if any error occurs
	 */
	public void write() throws BudgetFileException
	{
		
		try
		{
			write(new FileOutputStream(path));
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error writing to file", ioe);
			throw new BudgetFileException("Unable to save budget to file, " + path);
		}
	}
	
	/**
	 * Writes the budget to the given output stream
	 * 
	 * @param stream output stream to which to write the budget
	 * @throws BudgetFileException if any error occurs
	 */
	protected void write(OutputStream stream) throws BudgetFileException
	{
		try
		{
			writer.write(stream, budget);
			stream.close();
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error writing to stream", ioe);
			throw new BudgetFileException("Unable to write budget to stream, " + stream);
		}
	}

}
