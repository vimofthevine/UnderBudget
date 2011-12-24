package com.vimofthevine.underbudget.budget.file.parsers;

import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateDomParser;
import com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateParserFactory;
import com.vimofthevine.underbudget.budget.file.parsers.meta.BudgetMetaDomParser;
import com.vimofthevine.underbudget.budget.file.parsers.meta.BudgetMetaDomParserV1;
import com.vimofthevine.underbudget.budget.file.parsers.meta.BudgetMetaDomParserV3;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Budget file parser using the DOM XML parser
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFileDomParser implements BudgetFileParser {
	
	/**
	 * Log handle
	 */
	private static Logger logger = Logger.getLogger(BudgetFileDomParser.class.getName());
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * The budget parsed from the file
	 */
	protected Budget budget;
	
	/**
	 * Default constructor
	 */
	public BudgetFileDomParser()
	{
		logger.log(Level.FINE, "Initializing budget parser");
		
		progress = new TaskProgress();
	}

	@Override
    public TaskProgress getProgress()
    {
		return progress;
    }
	
	@Override
    public Budget getBudget()
    {
		return budget;
    }

	@Override
    public void parse(InputStream stream) throws BudgetFileException
    {
		try
		{
			progress.reset();
			
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(stream);
			
			int version = parseFileVersion(doc);
			BudgetMetaDomParser metaParser = createMetaParser(version);
			EstimateDomParser estimateParser = EstimateParserFactory.createDomParser(version, progress);
			
			BudgetMeta meta  = metaParser.parse(doc, 10);
			Estimate income  = estimateParser.parseIncomes(doc, 44);
			Estimate expense = estimateParser.parseExpenses(doc, 45);
			
			budget = new Budget(meta, income, expense);
		}
		catch (Exception e)
		{
			if (e instanceof BudgetFileException)
				throw (BudgetFileException) e;
			else
			{
				logger.log(Level.WARNING, "Exception parsing file", e);
				throw new BudgetFileException("Unable to read file");
			}
		}
    }
	
	/**
	 * Reads the file version from the root budget element,
	 * given a valid DOM document
	 * 
	 * @param doc parsed DOM document
	 * @return version of the current budget file
	 * @throws BudgetFileException if any error occurs
	 */
	protected int parseFileVersion(Document doc)
	throws BudgetFileException
	{
		try
		{
			// Get root budget element
			NodeList budgetList = doc.getElementsByTagName("budget");
			logger.log(Level.FINER, "Found " + budgetList.getLength()
				+ " budget elements");
			
			if (budgetList.getLength() < 1)
				return 1;
			
			Element budgetElement = (Element) budgetList.item(0);
			
			String version = budgetElement.getAttribute("version");
			logger.log(Level.FINER, "File version is " + version);
			
			return (version == "") ? 1 : Integer.parseInt(version);
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing file version", e);
			throw new BudgetFileException("Invalid budget definition in file");
		}
	}
	
	/**
	 * Creates a new budget meta parser according
	 * to the file version
	 * 
	 * @param version budget file version
	 * @return BudgetMetaDomParser
	 */
	public BudgetMetaDomParser createMetaParser(int version)
	{
		if (version < 3)
			return new BudgetMetaDomParserV1(progress);
		else
			return new BudgetMetaDomParserV3(progress);
	}

}
