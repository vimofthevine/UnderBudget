package com.vimofthevine.underbudget.report.export;

import java.io.OutputStream;
import java.math.BigDecimal;
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

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.report.WorksheetEntry;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * XML writer for exporting analysis results to an XML file
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ReportXmlWriter implements ReportWriter {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(ReportXmlWriter.class.getName());

	/**
	 * Current (latest) report version
	 */
	public static final int VERSION = 1;

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
	public ReportXmlWriter()
	{
		logger.log(Level.FINE, "Creating report XML writer");
		progress = new TaskProgress();
	}

	@Override
	public TaskProgress getProgress()
	{
		return progress;
	}

	@Override
	public void write(OutputStream stream, AnalysisResults results)
	throws ReportExportException
	{
		try
		{
			progress.reset();

			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			doc = db.newDocument();

			// Create root element
			Element rootElement = doc.createElement("report");
			rootElement.setAttribute("version", String.valueOf(VERSION));
			doc.appendChild(rootElement);

			// Summary report
			rootElement.appendChild(createSummaryElement(results, 25));

			// Comparison report
			rootElement.appendChild(createComparisonElement(results, 25));

			// Allocation report
			rootElement.appendChild(createAllocationElement(results, 25));

			// Worksheet report
			rootElement.appendChild(createWorksheetElement(results, 25));

			// Prepare source/output
			Source source = new DOMSource(doc);
			Result result = new StreamResult(stream);

			// Write XML to file
			logger.log(Level.FINE, "Writing report as XML");
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
			if (e instanceof ReportExportException)
				throw (ReportExportException) e;
			else
			{
				logger.log(Level.WARNING, "Error exporting report", e);
				throw new ReportExportException("Unable to export report to file");
			}
		}
	}
	
	/**
	 * Creates an XML element to represent the summary report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 * @return summary report element
	 */
	protected Element createSummaryElement(AnalysisResults results,
		float allocated)
	{
		logger.log(Level.FINE, "Creating summary report element");
		
		// Create summary element
		Element summaryElement = doc.createElement("summary-report");
		
		// Create start date element
		Element startElement = XmlHelper.createElement(doc, summaryElement, "start-date", null);
		XmlHelper.createDateElement(doc, startElement, results.budget.meta.getPeriod().getStartDate());
		
		// Create end date element
		Element endElement = XmlHelper.createElement(doc, summaryElement, "end-date", null);
		XmlHelper.createDateElement(doc, endElement, results.budget.meta.getPeriod().getEndDate());
		
		// Create initial balance element
		BigDecimal initial = results.budget.meta.getInitialBalance();
		XmlHelper.createElement(doc, summaryElement, "initial-balance", initial);
		
		// Create totals element
		Element totalElement = createTotalElement(summaryElement, results.total);
		
		// Create estimated balance element
		XmlHelper.createElement(doc, totalElement, "estimated-balance",
			initial.add(results.total.getEstimatedNetChange()));
		
		// Create actual balance element
		XmlHelper.createElement(doc, totalElement, "actual-balance",
			initial.add(results.total.getActualNetChange()));
		
		// Create expected balance element
		XmlHelper.createElement(doc, totalElement, "expected-balance",
			initial.add(results.total.getExpectedNetChange()));
		
		progress.add(allocated);
		
		return summaryElement;
	}
	
	/**
	 * Creates an XML element to represent the comparison report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 * @return comparison report element
	 */
	protected Element createComparisonElement(AnalysisResults results,
		float allocated)
	{
		logger.log(Level.FINE, "Creating comparison report element");
		
		// Create comparison element
		Element reportElement = doc.createElement("comparison-report");
		
		// Create income estimates element
		Element incomeElement = XmlHelper.createElement(doc, reportElement, "income-estimates", null);
		createEstimateElementTree(incomeElement, results.budget.incomes);
		createEstimateElementTree(incomeElement, results.rules.get(results.rules.size() - 2).estimate);
		
		// Create expense estimates element
		Element expenseElement = XmlHelper.createElement(doc, reportElement, "expense-estimates", null);
		createEstimateElementTree(expenseElement, results.budget.expenses);
		createEstimateElementTree(expenseElement, results.rules.get(results.rules.size() - 1).estimate);
		
		progress.add(allocated);
		
		return reportElement;
	}
	
	/**
	 * Creates an estimate element for a given estimate
	 * and its child estimates
	 * 
	 * @param parent   parent XML element
	 * @param estimate estimate to be written
	 */
	protected void createEstimateElementTree(Element parent, Estimate estimate)
	{
		Element estimateElement = createEstimateElement(parent, estimate);
		
		Element transElement = XmlHelper.createElement(doc, estimateElement, "transactions", null);
		
		for (Transaction transaction : estimate.getTransactions())
		{
			createTransactionElement(transElement, transaction);
		}
		
		Element estimatesElement = XmlHelper.createElement(doc, estimateElement, "estimates", null);		
		for (int i=0; i<estimate.getChildCount(); i++)
		{
			createEstimateElementTree(estimatesElement, estimate.getChildAt(i));
		}
	}
	
	/**
	 * Creates an XML element to represent the allocation report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 * @return allocation report element
	 */
	protected Element createAllocationElement(AnalysisResults results,
		float allocated)
	{
		logger.log(Level.FINE, "Creating allocation report element");
		
		// Create allocation element
		Element reportElement = doc.createElement("allocation-report");
		
		for (AllocationEntry entry : results.allocation)
		{
			Element entryElement = XmlHelper.createElement(doc, reportElement, "entry", null);
			
			createTransactionElement(entryElement, entry.transaction);
			createEstimateElement(entryElement, entry.estimate);
			XmlHelper.createElement(doc, entryElement, "rationale", entry.rationale);
		}
		
		progress.add(allocated);
		
		return reportElement;
	}
	
	/**
	 * Creates an XML element to represent the worksheet report
	 * 
	 * @param doc DOM   document to be populated
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 * @return worksheet report element
	 */
	protected Element createWorksheetElement(AnalysisResults results,
		float allocated)
	{
		logger.log(Level.FINE, "Creating allocation report element");
		
		// Create allocation element
		Element reportElement = doc.createElement("worksheet-report");
		
		for (WorksheetEntry entry : results.worksheet)
		{
			Element entryElement = XmlHelper.createElement(doc, reportElement, "entry", null);
			
			createEstimateElement(entryElement, entry.estimate);
			createTotalElement(entryElement, entry.totals);
			XmlHelper.createElement(doc, entryElement, "rationale", entry.rationele);
		}
		
		return reportElement;
	}
	
	/**
	 * Creates an estimate element for a given estimate
	 * 
	 * @param parent  parent XML element to contain the estimate
	 * @param estimate estimate to be written
	 * @return the new estimate element
	 */
	protected Element createEstimateElement(Element parent, Estimate estimate)
	{
		Element estimateElement = XmlHelper.createElement(doc, parent, "estimate", null);
		
		XmlHelper.createElement(doc, estimateElement, "name", estimate.getName());
		XmlHelper.createElement(doc, estimateElement, "notes", estimate.getNotes());
		XmlHelper.createElement(doc, estimateElement, "estimated-amount", estimate.getAmount());
		XmlHelper.createElement(doc, estimateElement, "actual-amount", estimate.getActualAmount());
		XmlHelper.createElement(doc, estimateElement, "difference", estimate.getDifferenceAmount());
		
		return estimateElement;
	}
	
	/**
	 * Creates a transaction element for a given transaction
	 * 
	 * @param parent      parent XML element
	 * @param transaction transaction to be written
	 */
	protected void createTransactionElement(Element parent, Transaction transaction)
	{
		Element transElement = XmlHelper.createElement(doc, parent, "transaction", null);
		
		Element dateElement = XmlHelper.createElement(doc, transElement, "date-posted", null);
		XmlHelper.createDateElement(doc, dateElement, transaction.date);
		
		XmlHelper.createElement(doc, transElement, "payee", transaction.payee);
		XmlHelper.createElement(doc, transElement, "memo", transaction.memo);
		XmlHelper.createElement(doc, transElement, "amount", transaction.value);
		XmlHelper.createElement(doc, transElement, "deposit-acct", transaction.deposit.getName());
		XmlHelper.createElement(doc, transElement, "withdrawal-acct", transaction.withdrawal.getName());
	}
	
	/**
	 * Creates a totals element for a given balance total
	 * 
	 * @param parent parent XML element
	 * @param total  balance total
	 * @return the new totals estimate
	 */
	protected Element createTotalElement(Element parent, BalanceTotal total)
	{
		Element totalElement = XmlHelper.createElement(doc, parent, "totals", null);
		
		XmlHelper.createElement(doc, totalElement, "estimated-income", total.estimatedIncome);
		XmlHelper.createElement(doc, totalElement, "estimated-expense", total.estimatedExpense);
		XmlHelper.createElement(doc, totalElement, "actual-income", total.actualIncome);
		XmlHelper.createElement(doc, totalElement, "actual-expense", total.actualExpense);
		XmlHelper.createElement(doc, totalElement, "expected-income", total.expectedIncome);
		XmlHelper.createElement(doc, totalElement, "expected-expense", total.expectedExpense);
		XmlHelper.createElement(doc, totalElement, "estimated-net", total.getEstimatedNetChange());
		XmlHelper.createElement(doc, totalElement, "actual-net", total.getActualNetChange());
		XmlHelper.createElement(doc, totalElement, "expected-net", total.getExpectedNetChange());
		
		return totalElement;
	}

}
