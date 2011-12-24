package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Document;
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
 * from a version 2 budget file (nested estimates)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV2 implements EstimateDomParser {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(EstimateDomParserV2.class.getName());

	/**
	 * Task progress
	 */
	TaskProgress progress;

	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public EstimateDomParserV2(TaskProgress progress)
	{
		this.progress = progress;
	}

	/**
	 * Extracts the given type of estimates from the
	 * given DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param type      income or expense
	 * @param allocated percentage of the overall task allocated
	 * @return root estimate, either income or expense
	 * @throws BudgetFileException if any error occurs
	 */
	protected Estimate parse(Document doc, String type, float allocated)
			throws BudgetFileException
			{
		try
		{
			// Get root estimate
			Element rootEstimateElement = (Element) ((Element) doc.getElementsByTagName(type + "-estimates")
					.item(0)).getElementsByTagName("estimate").item(0);

			// Create root estimate
			Estimate root = readEstimateFromElement(rootEstimateElement, null);

			// Read in nested estimates
			readNestedEstimates(root, rootEstimateElement, allocated);

			return root;
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing " + type + " estimates", e);
			throw new BudgetFileException("Invalid " + type + " estimates in file");
		}
			}

	/**
	 * Recursively read in the estimates under a given estimate root
	 * 
	 * @param parent    estimate to which to add the child elements
	 * @param element   XML DOM element from which to read
	 * @param allocated percentage of the overall task allocated to this task
	 * @throws BudgetFileException if any error occurs
	 */
	protected void readNestedEstimates(Estimate parent, Element element, float allocated)
	throws BudgetFileException
	{
		List<Element> estimateList = readChildEstimates(element);
		logger.log(Level.FINER, "Found " + estimateList.size()
				+ " estimates nested in " + parent.getName());

		float portion = allocated / (float) estimateList.size();

		// For each estimate nested inside the current element
		for (int i=0; i<estimateList.size(); i++)
		{
			logger.log(Level.FINEST, "Processing estimate #" + i);
			Element estimateElement = estimateList.get(i);
			Estimate estimate = readEstimateFromElement(estimateElement, parent);

			// If estimate contains nested estimates
			if (estimateElement.getElementsByTagName("estimates").getLength() > 0)
			{
				readNestedEstimates(estimate, estimateElement, portion);
			}
			else
			{
				progress.add(portion);
			}
		}
	}

	/**
	 * Reads in the estimate elements that are direct
	 * children to a given DOM element
	 * 
	 * @param element the DOM element from which to read
	 * @return list of DOM elements representing child estimates
	 */
	protected List<Element> readChildEstimates(Element element)
	{
		List<Element> elements = new ArrayList<Element>();

		Element estimatesElement = (Element) element.getElementsByTagName("estimates").item(0);
		NodeList estimateList = estimatesElement.getElementsByTagName("estimate");

		for (int i=0; i<estimateList.getLength(); i++)
		{
			Element estimate = (Element) estimateList.item(i);
			if (estimate.getParentNode() == estimatesElement)
			{
				elements.add(estimate);
			}
		}

		return elements;
	}

	/**
	 * Reads in an estimate from the XML structure, adding it to
	 * the given parent estimate
	 * 
	 * @param element XML DOM element to read
	 * @param parent  parent estimate to add the read estimate
	 * @return the newly created estimate
	 * @throws BudgetFileException if any error occurs
	 */
	protected Estimate readEstimateFromElement(Element element, Estimate parent)
	throws BudgetFileException
	{
		Estimate estimate = new Estimate();
		
		// Check if estimate is a category
		boolean isCategory = (element.getElementsByTagName("estimates").getLength() > 0);
		
		estimate.setName(XmlHelper.readTextNode(element, "name"));
		estimate.setNotes(XmlHelper.readTextNode(element, "notes"));
		
		if ( ! isCategory)
		{
			estimate.setAmount(new BigDecimal(XmlHelper.readTextNode(element, "amount")));
			estimate.setFinal(Boolean.parseBoolean(XmlHelper.readTextNode(element, "final")));
			readRulesForEstimate(estimate, element);
		}

		if (parent != null)
		{
			parent.add(estimate);
		}

		return estimate;
	}

	/**
	 * Reads the match rules for the given estimate
	 */
	protected void readRulesForEstimate(Estimate estimate, Element element)
	throws BudgetFileException
	{
		String matches = XmlHelper.readTextNode(element, "matches");

		// Add rule only if match text is given
		if ( ! matches.equals(""))
		{
			estimate.addRule(new Rule(TransactionField.ANY,
					ComparisonOperator.CONTAINS, matches));
		}
	}

	@Override
	public Estimate parseIncomes(Document doc, float allocated)
	throws BudgetFileException
	{
		return parse(doc, "income", allocated);
	}

	@Override
	public Estimate parseExpenses(Document doc, float allocated)
	throws BudgetFileException
	{
		return parse(doc, "expense", allocated);
	}

}
