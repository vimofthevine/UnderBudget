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

package com.vimofthevine.underbudget.transactions.file.writers;

import java.io.OutputStream;
import java.util.List;
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

import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.TransactionFileException;
import com.vimofthevine.underbudget.util.XmlHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Transaction file writer using the DOM XML API
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFileDomWriter implements TransactionFileWriter {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TransactionFileDomWriter.class.getName());
	
	/**
	 * Current (latest) file version
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
	public TransactionFileDomWriter()
	{
		logger.log(Level.FINE, "Initializing transaction file writer");
		
		progress = new TaskProgress();
	}

	@Override
    public TaskProgress getProgress()
    {
		return progress;
    }

	@Override
    public void write(OutputStream stream, List<Transaction> transactions)
    throws TransactionFileException
    {
		try
		{
			progress.reset();
			
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			doc = db.newDocument();
			
			// Create root element
			Element rootElement = doc.createElement("transactions");
			rootElement.setAttribute("version", String.valueOf(VERSION));
			doc.appendChild(rootElement);
			
			float portion = 100 / transactions.size();
			
			// Add transactions
			for (Transaction transaction : transactions)
			{
				createTransactionElement(rootElement, transaction);
				progress.add(portion);
			}
			
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
			if (e instanceof TransactionFileException)
				throw (TransactionFileException) e;
			else
			{
				logger.log(Level.WARNING, "Exception writing file", e);
				throw new TransactionFileException("Unable to write file");
			}
		}
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

}
