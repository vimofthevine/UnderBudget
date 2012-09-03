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

package com.vimofthevine.underbudget.xml.budget.source;

import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.xml.budget.XmlBudget;

/**
 * Source for a budget stored as XML, using the
 * Simple XML framework for serialization and
 * deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetXmlFileSource implements BudgetSource {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetXmlFileSource.class.getName());
	
	/**
	 * Budget XML file
	 */
	private final File xmlFile;
	
	/**
	 * Simple XML serializer
	 */
	private final Serializer serializer;
	
	/**
	 * Budget from the source
	 */
	private XmlBudget budget;
	
	/**
	 * Constructs a new XML file budget source.
	 * 
	 * @param file budget XML file
	 */
	public BudgetXmlFileSource(File file)
	{
		xmlFile = file;
		serializer = BudgetSerializerFactory.createSerializer();
	}
	
	/**
	 * Constructs a new XML file budget source,
	 * with a budget to be saved to the source.
	 * 
	 * @param file         budget XML file
	 * @param budgetToCopy budget to be saved
	 */
	public BudgetXmlFileSource(File file, Budget budgetToCopy)
	{
		xmlFile = file;
		budget = (budgetToCopy instanceof XmlBudget)
			? (XmlBudget) budgetToCopy : new XmlBudget(budgetToCopy);
		serializer = BudgetSerializerFactory.createSerializer();
	}

	@Override
    public Budget getBudget() throws BudgetSourceException
    {
		try
		{
			budget = serializer.read(XmlBudget.class, xmlFile);
			return budget;
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to parse XML budget file", e);
		}
    }

	@Override
    public void persist() throws BudgetSourceException
    {
		logger.log(Level.INFO, "Saving to " + xmlFile.getAbsolutePath());
		
		/*
		if ( ! xmlFile.exists())
		{
			try
			{
				xmlFile.createNewFile();
			}
			catch (IOException ioe)
			{
				throw new BudgetSourceException("Unable to save budget to "
					+ xmlFile.getAbsolutePath(), ioe);
			}
		}
		*/
		
		if (budget == null)
			throw new BudgetSourceException("No budget to be saved");
		
		try
		{
			serializer.write(budget, xmlFile);
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to save budget", e);
		}
    }

}