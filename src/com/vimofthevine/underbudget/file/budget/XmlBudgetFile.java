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

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.progress.ProgressMonitorInputStream;
import com.vimofthevine.underbudget.core.progress.TaskProgress;
import com.vimofthevine.underbudget.file.xml.BudgetSerializerFactory;

/**
 * Budget file stored as xml, using the Simple XML
 * framework for reading and writing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlBudgetFile implements BudgetFile {
	
	/**
	 * Budget file path
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
	 * Constructs a new XML budget file
	 * 
	 * @param path    path to the budget file
	 * @param factory currency factory
	 */
	public XmlBudgetFile(String path, CurrencyFactory factory)
	{
		this.path = path;
		this.currencyFactory = factory;
		
		serializer = BudgetSerializerFactory.createSerializer();
	}

	@Override
    public Budget read(TaskProgress progress) throws BudgetFileException
    {
		try
		{
			Budget empty = new Budget("New Budget", new MonthlyBudgetingPeriod(),
				currencyFactory.newCurrencyInstance(), null, null);
			return serializer.read(empty, new ProgressMonitorInputStream(
				progress, new FileInputStream(path)));
		}
		catch (FileNotFoundException fnfe)
		{
			throw new BudgetFileException("File, " + path + ", not found");
		}
		catch (Exception e)
		{
			throw new BudgetFileException("Unable to parse budget", e);
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
			throw new BudgetFileException("Unable to save budget", e);
		}
    }
	
}
