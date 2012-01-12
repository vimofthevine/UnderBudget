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
import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileParser;
import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileParserFactory;
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
	 * Constructor, specifying the path
	 * to the budget file
	 * 
	 * @param path file path
	 */
	public BudgetFile(String path)
	{
		this.path = path;
		
		parser = BudgetFileParserFactory.createParser();
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
	 * Parses the budget file, extracting the
	 * budget defined in the file
	 * 
	 * @throws BudgetFileException if any error occurs
	 */
	public void parse() throws BudgetFileException
	{
		try
		{
			InputStream stream = new FileInputStream(path);
			parser.parse(stream);
			budget = parser.getBudget();
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error opening file", ioe);
			throw new BudgetFileException("Unable to open budget file, " + path);
		}
		
	}

}
