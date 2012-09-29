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
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.DefaultCustomPeriod;
import com.vimofthevine.underbudget.core.budget.period.LiteralMonthPeriod;
import com.vimofthevine.underbudget.core.budget.period.PaydateMonthPeriod;
import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.xml.budget.XmlBudget;
import com.vimofthevine.underbudget.xml.budget.XmlTemplateBudget;

/**
 * Source for the template budget. The template is
 * read from an XML resource file, if an XML file
 * does not exist in the settings directory. The
 * Simple XML framework is used for serialization
 * and deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TemplateBudgetSource implements BudgetSource {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TemplateBudgetSource.class.getName());
	
	/**
	 * Template budget XML file
	 */
	private final File templateFile;
	
	/**
	 * Simple XML serializer
	 */
	private final Serializer serializer;
	
	/**
	 * Budget to replace the template (if set)
	 */
	private final XmlBudget newBudget;
	
	/**
	 * Constructs a new template budget source.
	 * 
	 * @param settingsDir application settings directory
	 */
	public TemplateBudgetSource(String settingsDir)
	{
		templateFile = new File(settingsDir + "/template.xml");
		serializer = BudgetSerializerFactory.createSerializer();
		newBudget = null;
	}
	
	/**
	 * Constructs a new template budget source,
	 * with a budget to replace the template.
	 * 
	 * @param settingsDir  application settings directory
	 * @param budgetToCopy new template budget
	 */
	public TemplateBudgetSource(String settingsDir, Budget budgetToCopy)
	{
		templateFile = new File(settingsDir + "/template.xml");
		serializer = BudgetSerializerFactory.createSerializer();
		newBudget = new XmlTemplateBudget(budgetToCopy);
	}

	@Override
	public Budget getBudget() throws BudgetSourceException
	{
		MutableBudget template;
		
		try
		{
			if (templateFile.exists())
			{
				logger.log(Level.INFO, "Reading template from " + templateFile.getAbsolutePath());
				template = serializer.read(XmlBudget.class, templateFile);
			}
			else
			{
				logger.log(Level.INFO, "Reading built-in template");
				InputStream templateStream = getClass().getResourceAsStream("template.xml");
				template = serializer.read(XmlBudget.class, templateStream);
			}
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to parse template budget", e);
		}
		
		if (template != null)
		{
			// Create an up-to-date budgeting period
			final BudgetingPeriod currentPeriod = createCurrentPeriod(
				template.getDefinition().getPeriod().getType());
			final BudgetDefinition definition = template.getDefinition();
			template.setDefinition(new BudgetDefinition() {
				@Override
                public String getName() { return definition.getName(); }
				@Override
                public CashCommodity getInitialBalance() { return definition.getInitialBalance(); }
				@Override
                public BudgetingPeriod getPeriod() { return currentPeriod; }
			});
		}
		
		return template;
	}
	
	/**
	 * Creates a budgeting period of the specified type,
	 * initialized to "current" values (i.e., this month, etc.).
	 * 
	 * @param type period type
	 * @return current budgeting period
	 */
	private BudgetingPeriod createCurrentPeriod(PeriodType type)
	{
		switch (type)
		{
			case LITERAL_MONTH:
				return new LiteralMonthPeriod();
			case PAYDATE_MONTH:
				return new PaydateMonthPeriod();
			case CUSTOM:
				return new DefaultCustomPeriod();
			default:
				return new LiteralMonthPeriod();
		}
	}

	@Override
	public void persist() throws BudgetSourceException
	{
		if (newBudget != null)
		{
			logger.log(Level.INFO, "Updating template at " + templateFile.getAbsolutePath());
			
			try
			{
				serializer.write(newBudget, templateFile);
			}
			catch (Exception e)
			{
				throw new BudgetSourceException("Unable to update template", e);
			}
		}
		else
		{
			logger.log(Level.INFO, "No budget given to replace template");
		}
	}
	
	@Override
	public String getDescription()
	{
		return "Template";
	}

}
