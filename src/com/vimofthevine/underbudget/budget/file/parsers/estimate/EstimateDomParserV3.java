package com.vimofthevine.underbudget.budget.file.parsers.estimate;

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
