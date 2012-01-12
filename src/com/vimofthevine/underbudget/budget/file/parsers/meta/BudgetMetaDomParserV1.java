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

package com.vimofthevine.underbudget.budget.file.parsers.meta;

import java.math.BigDecimal;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * DOM parser for extracting budget meta data
 * from a pre-version-3 budget file
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMetaDomParserV1 implements BudgetMetaDomParser {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetMetaDomParserV1.class.getName());
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public BudgetMetaDomParserV1(TaskProgress progress)
	{
		this.progress = progress;
	}
	
	/**
	 * Extracts the budget meta info defined in the given
	 * DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return budget meta data defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public BudgetMeta parse(Document doc, float allocated)
	throws BudgetFileException
	{
		try
		{
			// Get meta element
			NodeList metaList = doc.getElementsByTagName("meta");
			logger.log(Level.FINER, "Found " + metaList.getLength()
				+ " meta data elements");
			
			if (metaList.getLength() < 1)
				return new BudgetMeta();
			
			Element metaElement = (Element) metaList.item(0);
			
			String name = parseName(metaElement);
			BigDecimal initial = parseInitialBalance(metaElement);
			BudgetingPeriod period = parseBudgetingPeriod(metaElement);
			
			progress.add(allocated);
			
			return new BudgetMeta(name, period, initial);
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing meta data", e);
			throw new BudgetFileException("Invalid meta data in file");
		}
	}
	
	/**
	 * Parses the budget name from the given XML element
	 * 
	 * @param element the XML element to parse
	 * @return name of the budget
	 * @throws BudgetFileException if any error occurs
	 */
	protected String parseName(Element element)
	throws BudgetFileException
	{
		return XmlHelper.readTextNode(element, "name");
	}
	
	/**
	 * Parses the initial balance from the given XML document
	 * 
	 * @param element the XML element to parse
	 * @return initial balance
	 * @throws BudgetFileException if any error occurs
	 */
	protected BigDecimal parseInitialBalance(Element element)
	throws BudgetFileException
	{
		return new BigDecimal(XmlHelper.readTextNode(element, "initialBalance"));
	}
	
	/**
	 * Parses the budgeting period from the given XML document
	 * 
	 * @param element the XML element to parse
	 * @return budgeting period
	 * @throws BudgetFileException if any error occurs
	 */
	protected BudgetingPeriod parseBudgetingPeriod(Element element)
	throws BudgetFileException
	{
		int month       = Integer.parseInt(XmlHelper.readTextNode(element, "month"));
		int year        = Integer.parseInt(XmlHelper.readTextNode(element, "year"));

		return new MonthlyBudgetingPeriod(month, year);
	}

}
