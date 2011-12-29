package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import java.math.BigDecimal;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.ExpenseEstimate;
import com.vimofthevine.underbudget.estimates.IncomeEstimate;
import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * DOM parser for extracting estimate definitions
 * from a version 1 budget file (parent ID)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV1 implements EstimateDomParser {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(EstimateDomParserV1.class.getName());
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public EstimateDomParserV1(TaskProgress progress)
	{
		this.progress = progress;
	}
	
	/**
	 * Extracts the given type of estimates from the given
	 * DOM document
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
			// Create root estimate
			Estimate root = new Estimate();
			
			// Get estimate elements
			NodeList estimateList = ((Element) doc.getElementsByTagName(type + "-estimates")
				.item(0)).getElementsByTagName("estimate");
			logger.log(Level.FINER, "Found " + estimateList.getLength()
				+ " " + type + " estimate elements");
			
			// Calculate task progress for each estimate
			float portion = allocated / (float) estimateList.getLength();
			
			HashMap<String,Estimate> estimates = new HashMap<String,Estimate>();
			
			for (int i=0; i<estimateList.getLength(); i++)
			{
				logger.log(Level.FINEST, "Processing estimate #" + i);
				Element estimateElement = (Element) estimateList.item(i);
				
				String id     = estimateElement.getAttribute("id");
				String parent = estimateElement.getAttribute("parent");
				
				String name       = XmlHelper.readTextNode(estimateElement, "name");
				String notes      = XmlHelper.readTextNode(estimateElement, "notes");
				String matches    = XmlHelper.readTextNode(estimateElement, "matches");
				BigDecimal amount = new BigDecimal(XmlHelper.readTextNode(estimateElement, "amount"));
				boolean isFinal   = Boolean.parseBoolean(XmlHelper.readTextNode(estimateElement, "final"));
				
				// Root estimate definition
				if (parent.equals("") || parent.equals("-1"))
				{
					if ( ! root.getName().equals(""))
						throw new BudgetFileException("Multiple " + type + " root estimates found");
					
					logger.log(Level.FINER, "Root " + type + " estimate found");
					
					root.setName(name);
					root.setNotes(notes);
					root.setAmount(amount);
					root.setFinal(isFinal);
					
					// Add rule only if match text is given
					if ( ! matches.equals(""))
					{
						String[] rules = matches.split(",");
						
						for (int j=0; j<rules.length; j++)
						{
							root.addRule(new Rule(TransactionField.ANY,
								ComparisonOperator.CONTAINS, rules[j]));
						}
					}
					
					estimates.put(id, root);
				}
				else
				{
					Estimate parentEstimate = estimates.get(parent);
					
					if (parentEstimate == null)
					{
						logger.log(Level.SEVERE, "Parent estimate not found for " + name);
						throw new BudgetFileException(type + " estimate missing parent");
					}
					
					Estimate estimate = (type.equalsIgnoreCase("income")
						? new IncomeEstimate() : new ExpenseEstimate());
					
					estimate.setName(name);
					estimate.setNotes(notes);
					estimate.setAmount(amount);
					estimate.setFinal(isFinal);
					
					// Add rule only if match text is given
					if ( ! matches.equals(""))
					{
						String[] rules = matches.split(",");
						
						for (int j=0; j<rules.length; j++)
						{
							estimate.addRule(new Rule(TransactionField.ANY,
								ComparisonOperator.CONTAINS, rules[j]));
						}
					}
					
					parentEstimate.add(estimate);
					
					estimates.put(id, estimate);
				}
				
				progress.add(portion);
			}
			
			return root;
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing " + type + " estimates", e);
			throw new BudgetFileException("Invalid " + type + " estimates in file");
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
