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

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.file.writers.BudgetFileWriter;
import com.vimofthevine.underbudget.budget.file.writers.BudgetFileWriterFactory;

/**
 * Representation of the template budget file
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TemplateFile extends BudgetFile {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TemplateFile.class.getName());
	
	/**
	 * System property specifying the template file location
	 */
	public static final String LOCATION = "underbudget.template.file";

	/**
	 * Constructor
	 * 
	 * Determines the location
	 */
	public TemplateFile()
	{
		super(null);
		
		path = System.getProperty(LOCATION, "./template.xml");
	}
	
	@Override
	public void parse() throws BudgetFileException
	{
		try
		{
			File templateFile = new File(path);
			
			// Read from the file if it exists, or read from the resource
			InputStream templateStream = templateFile.exists()
				? new FileInputStream(templateFile)
				: Application.class.getResourceAsStream("resources/template.xml");
				
			parse(templateStream);
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error parsing template", ioe);
			throw new BudgetFileException("Unable to parse template");
		}
	}
	
	/**
	 * Writes the template budget to the new budget file specified
	 * 
	 * @param path file name of the new file to be written
	 * @throws BudgetFileException if any error occurs
	 */
	public void write(String path) throws BudgetFileException
	{
		// Need to save this so it can be set back when we're done
		String templateFilePath = this.path;
		
		try
		{
			// Use the specified file path when writing
    		this.path = path;
    		write();
		}
		finally
		{
			// Re-set the template file path
			this.path = templateFilePath;
		}
	}
	
	/**
	 * Writes the given budget as the new template budget
	 * 
	 * @param budget new template budget
	 * @throws BudgetFileException if any error occurs
	 */
	public void write(Budget budget) throws BudgetFileException
	{
		this.budget = budget;
		
		// Need to save this so it can be set back when we're done
		BudgetFileWriter origWriter = writer;
		
		try
		{
			// Use the template budget file writer
			writer = BudgetFileWriterFactory.createTemplateWriter();
			write();
		}
		finally
		{
			// Re-set the normal budget file writer
			writer = origWriter;
		}
	}
	
}
