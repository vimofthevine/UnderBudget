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
import java.io.FileNotFoundException;
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
	 * Lock file
	 */
	private final File lockFile;
	
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
		lockFile = obtainLock(file);
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
		lockFile = obtainLock(file);
	}
	
	/**
	 * Obtains a lock on the given file. If a lock
	 * already exists, <code>null</code> is returned,
	 * else the handle to the lock file is returned.
	 * 
	 * @param file file attempting to be opened
	 * @return lock file handle, or <code>null</code>
	 *         if the file is already locked
	 */
	private File obtainLock(File file)
	{
		File lock = new File(file.getAbsolutePath() + ".lock");
		if (lock.exists())
			return null;
		
		try
		{
			lock.createNewFile();
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Unable to create lock file for " + file.getAbsolutePath());
		}
		
		return lock;
	}
		
	@Override
    public Budget retrieve() throws BudgetSourceException
    {
		if (lockFile == null)
		{
			throw new BudgetSourceException("File, " + xmlFile.getAbsolutePath()
				+ " is already open");
		}
	
		if ( ! xmlFile.exists())
		{
			throw new BudgetSourceException("File, " + xmlFile.getAbsolutePath()
				+ " does not exist");
		}
		
		if ( ! xmlFile.canRead())
		{
			throw new BudgetSourceException("Cannot read file, "
				+ xmlFile.getAbsolutePath());
		}
		
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
		if (lockFile == null)
		{
			throw new BudgetSourceException("File, " + xmlFile.getAbsolutePath()
				+ " is already open");
		}
		
		logger.log(Level.FINE, "Saving to " + xmlFile.getAbsolutePath());
		
		if (budget == null)
			throw new BudgetSourceException("No budget to be saved");
		
		if (xmlFile.exists() && ! xmlFile.canWrite())
		{
			throw new BudgetSourceException("Cannot write to file, "
				+ xmlFile.getAbsolutePath());
		}
		
		try
		{
			serializer.write(budget, xmlFile);
		}
		catch (FileNotFoundException fnfe)
		{
			throw new BudgetSourceException("Could not create file, "
				+ xmlFile.getAbsolutePath());
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to save budget", e);
		}
    }
	
	@Override
	public void close() throws BudgetSourceException
	{
		if (lockFile != null)
		{
			lockFile.delete();
		}
	}
	
	@Override
	public String getDescription()
	{
		return (xmlFile == null) ? "New XML file" : xmlFile.getAbsolutePath();
	}

}
