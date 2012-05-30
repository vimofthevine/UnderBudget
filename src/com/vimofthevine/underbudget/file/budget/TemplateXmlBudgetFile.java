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

package com.vimofthevine.underbudget.file.budget;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.progress.ProgressMonitorInputStream;
import com.vimofthevine.underbudget.core.progress.TaskProgress;
import com.vimofthevine.underbudget.file.xml.BudgetSerializerFactory;

/**
 * Template budget stored as an XML file, using the
 * Simple xml framework for reading and writing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TemplateXmlBudgetFile implements BudgetFile {

	/**
	 * System property specifying the template file location
	 */
	public static final String LOCATION = "underbudget.template.file";
	
	/**
	 * Template budget file path
	 */
	private final String path;
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory currencyFactory;
	
	/**
	 * Simple xml serializer
	 */
	private final Serializer serializer;
	
	/**
	 * Constructs a new template XML budget file.
	 * <p>
	 * The location of the template file is obtained from
	 * the system property, <code>underbudget.template.file</code>,
	 * defaulting to the file "template.xml" in the current
	 * directory if the system property is not set.
	 * 
	 * @param factory currency factory
	 */
	public TemplateXmlBudgetFile(CurrencyFactory factory)
	{
		path = System.getProperty(LOCATION, "./template.xml");
		currencyFactory = factory;
		
		serializer = BudgetSerializerFactory.createSerializer();
	}

	@Override
    public Budget read(TaskProgress progress) throws BudgetFileException
    {
		try
		{
			File templateFile = new File(path);
			Budget empty = new Budget("New Budget", new MonthlyBudgetingPeriod(),
				currencyFactory.newCurrencyInstance(), null, null);
			
			if (templateFile.exists())
			{
				// Read from the template file if it exists
				return serializer.read(empty, new ProgressMonitorInputStream(
					progress, new FileInputStream(path)));
			}
			else
			{
				// Otherwise use the default template
				InputStream defaultTemplate = Application.class.getResourceAsStream("resources/template.xml");
				return serializer.read(empty, new ProgressMonitorInputStream(
					progress, defaultTemplate));
			}
		}
		catch (FileNotFoundException fnfe)
		{
			throw new BudgetFileException("File, " + path + ", not found");
		}
		catch (Exception e)
		{
			throw new BudgetFileException("Unable to parse template", e);
		}
    }

	@Override
    public void save(Budget budget, TaskProgress progress)
            throws BudgetFileException
    {
		try
		{
			serializer.write(budget, new FileOutputStream(path));
		}
		catch (FileNotFoundException fnfe)
		{
			throw new BudgetFileException("File, " + path + ", not found");
		}
		catch (Exception e)
		{
			throw new BudgetFileException("Unable to update template", e);
		}
    }
	
}
