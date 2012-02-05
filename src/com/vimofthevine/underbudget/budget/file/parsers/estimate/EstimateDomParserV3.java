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

package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import java.util.Calendar;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * DOM parser for extracting estimate definitions
 * from a version 3 budget file (complex match rules)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV3 extends EstimateDomParserV2 {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(EstimateDomParserV3.class.getName());

	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public EstimateDomParserV3(TaskProgress progress)
	{
		super(progress);
	}
	
	@Override
	protected Estimate readEstimateFromElement(Element element, Estimate parent)
	throws BudgetFileException
	{
		Estimate estimate = super.readEstimateFromElement(element, parent);

		NodeList dueDateList = element.getElementsByTagName("due");
		if (dueDateList.getLength() > 0)
		{
			Calendar calendar = Calendar.getInstance();
			calendar.set(Calendar.DAY_OF_MONTH, Integer.valueOf(XmlHelper.readTextNode(element, "due")));
			estimate.setDueDate(calendar.getTime());
		}
		
		NodeList discreteList = element.getElementsByTagName("discrete");
		if (discreteList.getLength() > 0)
			estimate.setDiscrete(true);
		
		NodeList finalList = element.getElementsByTagName("final");
		if (finalList.getLength() > 0)
			estimate.setFinal(true);
		
		return estimate;
	}
	
	@Override
	protected void readRulesForEstimate(Estimate estimate, Element element)
	throws BudgetFileException
	{
		NodeList rulesList = element.getElementsByTagName("rules");
		
		// Make sure only one rule list is found
		if (rulesList.getLength() > 1)
			throw new BudgetFileException("Invalid rules found for estimate " + estimate.getName());
		
		// If no rules defined, do nothing
		if (rulesList.getLength() == 0)
			return;
		
		NodeList ruleList = ((Element) rulesList.item(0)).getElementsByTagName("rule");
		logger.log(Level.FINER, "Found " + ruleList.getLength()
			+ " rule definitions");
		
		for (int i=0; i<ruleList.getLength(); i++)
		{
			Element ruleElement = (Element) ruleList.item(i);
			
			String field    = XmlHelper.readTextNode(ruleElement, "field");
			String operator = XmlHelper.readTextNode(ruleElement, "operator");
			String text     = XmlHelper.readTextNode(ruleElement, "text");
			
			estimate.addRule(new Rule(
				TransactionField.getValueOf(field),
				ComparisonOperator.getValueOf(operator),
				text
			));
		}
	}

}
