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

package com.vimofthevine.underbudget.budget.file.writers;

import java.io.OutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Budget file writer using the DOM XML API
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFileDomWriter implements BudgetFileWriter {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetFileDomWriter.class.getName());
	
	/**
	 * Current (latest) file version
	 */
	public static final int VERSION = 3;
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * DOM document in use
	 */
	protected Document doc;
	
	/**
	 * Default constructor
	 */
	public BudgetFileDomWriter()
	{
		logger.log(Level.FINE, "Initializing budget file writer");
		
		progress = new TaskProgress();
	}
	
	@Override
    public TaskProgress getProgress()
    {
		return progress;
    }
	
	@Override
	public void write(OutputStream stream, Budget budget)
	throws BudgetFileException
	{
		try
		{
			progress.reset();
			
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			doc = db.newDocument();
			
			// Create root element
			Element rootElement = doc.createElement("budget");
			rootElement.setAttribute("version", String.valueOf(VERSION));
			doc.appendChild(rootElement);
			
			rootElement.appendChild(createMetaElement(budget, 10));
			rootElement.appendChild(createIncomeEstimatesElement(budget, 44));
			rootElement.appendChild(createExpenseEstimatesElement(budget, 44));
			
			// Prepare source/output
			Source source = new DOMSource(doc);
			Result result = new StreamResult(stream);
			
			// Write XML to file
			logger.log(Level.FINE, "Writing budget as XML");
			TransformerFactory tFactory = TransformerFactory.newInstance();
			tFactory.setAttribute("indent-number", new Integer(2));
			Transformer xformer = tFactory.newTransformer();
			xformer.setOutputProperty(OutputKeys.INDENT, "yes");
			xformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
			xformer.transform(source, result);
			
			progress.complete();
		}
		catch (Exception e)
		{
			if (e instanceof BudgetFileException)
				throw (BudgetFileException) e;
			else
			{
				logger.log(Level.WARNING, "Exception writing file", e);
				throw new BudgetFileException("Unable to write to file");
			}
		}
	}
	
	/**
	 * Creates a meta element for the budget
	 * 
	 * @param budget    budget whose meta is to be written
	 * @param allocated percentage of the overall task allocated to this task
	 * @return meta DOM element
	 */
	protected Element createMetaElement(Budget budget, float allocated)
	{
		logger.log(Level.FINE, "Creating meta element");
		
		Element metaElement = doc.createElement("meta");
		
		XmlHelper.createElement(doc, metaElement, "name", budget.meta.getName());
		XmlHelper.createElement(doc, metaElement, "initialBalance", budget.meta.getInitialBalance());
		
		Element periodElement = XmlHelper.createElement(doc, metaElement, "period", null);
		budget.meta.getPeriod().store(doc, periodElement);
		
		progress.add(allocated);
		
		return metaElement;
	}
	
	/**
	 * Creates an income estimates element
	 * 
	 * @param budget    budget whose income estimates are to be written
	 * @param allocated percentage of the overall task allocated to this task
	 * @return income estimates DOM element
	 */
	protected Element createIncomeEstimatesElement(Budget budget, float allocated)
	{
		logger.log(Level.FINE, "Creating root income estimates element");
		
		Element incomesElement = doc.createElement("income-estimates");
		
		// Add root income estimate first
		incomesElement.appendChild(createEstimateElement(budget.incomes));
		
		return incomesElement;
	}
	
	/**
	 * Creates an expense estimates element
	 * 
	 * @param budget    budget whose expense estimates are to be written
	 * @param allocated percentage of the overall task allocated to this task
	 * @return expense estimates DOM element
	 */
	protected Element createExpenseEstimatesElement(Budget budget, float allocated)
	{
		logger.log(Level.FINE, "Creating root expense estimates element");
		
		Element expensesElement = doc.createElement("expense-estimates");
		
		expensesElement.appendChild(createEstimateElement(budget.expenses));
			
		return expensesElement;
	}
	
	/**
	 * Creates an estimate element
	 * 
	 * @param estimate the estimate to be written
	 * @return estimate DOM element
	 */
	protected Element createEstimateElement(Estimate estimate)
	{
		logger.log(Level.FINEST, "Creating estimate element");
		
		Element estimateElement = doc.createElement("estimate");
		
		populateEstimateMeta(estimate, estimateElement);
		
		if ( ! estimate.isCategory())
		{

			if (estimate.getRules().size() > 0)
			{
				Element rulesElement = XmlHelper.createElement(doc, estimateElement, "rules", null);
				
				for (Rule rule : estimate.getRules())
				{
					rulesElement.appendChild(createRuleElement(rule));
				}
			}
		}
		else
		{
			Element estimatesElement = XmlHelper.createElement(doc, estimateElement, "estimates", null);
			
			for (int i=0; i<estimate.getChildCount(); i++)
			{
				Estimate child = estimate.getChildAt(0);
				estimatesElement.appendChild(createEstimateElement(child));
			}
		}
		
		return estimateElement;
	}
	
	/**
	 * Populates an estimate element with the estimate meta data
	 * 
	 * @param estimate estimate to be written
	 * @param element  element to be populated
	 */
	protected void populateEstimateMeta(Estimate estimate, Element element)
	{
		XmlHelper.createElement(doc, element, "name", estimate.getName());
		XmlHelper.createElement(doc, element, "notes", estimate.getNotes());
		
		if ( ! estimate.isCategory())
		{
			XmlHelper.createElement(doc, element, "amount", String.valueOf(estimate.getAmount()));
			XmlHelper.createElement(doc, element, "final", String.valueOf(estimate.isFinal()));
		}
	}
	
	/**
	 * Creates a rule element
	 * 
	 * @param rule the rule to be written
	 * @return rule DOM element
	 */
	protected Element createRuleElement(Rule rule)
	{
		logger.log(Level.FINEST, "Creating rule element");
		
		Element ruleElement = doc.createElement("rule");
		
		XmlHelper.createElement(doc, ruleElement, "field", rule.getField());
		XmlHelper.createElement(doc, ruleElement, "operator", rule.getOperator());
		XmlHelper.createElement(doc, ruleElement, "text", rule.getText());
		
		return ruleElement;
	}
	
}
