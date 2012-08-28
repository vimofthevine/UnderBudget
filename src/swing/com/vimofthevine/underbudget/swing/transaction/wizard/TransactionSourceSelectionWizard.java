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

package com.vimofthevine.underbudget.swing.transaction.wizard;

import java.awt.Frame;
import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.transaction.source.GnuCashXmlFileSource;
import com.vimofthevine.underbudget.stubs.transaction.source.StubTransactionSource;
import com.vimofthevine.underbudget.swing.transaction.events.SelectTransactionSourceEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionSourceSelectedEvent;

/**
 * Wizard for selecting a source for importing transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSourceSelectionWizard {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory currency;
	
	/**
	 * Constructs a new transaction source selection wizard.
	 * 
	 * @param bus     event bus
	 * @param parent  application window
	 * @param factory currency factory instance
	 */
	public TransactionSourceSelectionWizard(EventBus bus, Frame parent,
		CurrencyFactory factory)
	{
		eventBus = bus;
		eventBus.register(this);
		
		window = parent;
		
		currency = factory;
	}
	
	@Subscribe
	public void selectSource(SelectTransactionSourceEvent event)
	{
		new SourceTypeSelectionDialog(window, this);
	}
	
	void typeSelected(SourceType type)
	{
		switch (type)
		{
			case GNUCASH_XML:
				SwingUtilities.invokeLater(new Runnable() {
					public void run()
					{
        				GnuCashXmlFileChooser chooser = new GnuCashXmlFileChooser();
        				int result = chooser.showOpenDialog(window);
        				
        				if (result == JFileChooser.APPROVE_OPTION)
        				{
        					final File file = chooser.getSelectedFile();
        					
        					// Get off EDT
        					new Thread() {
        						public void run()
        						{
        							try
        							{
            							GnuCashXmlFileSource source =
            								new GnuCashXmlFileSource(file, currency);
            							eventBus.post(new TransactionSourceSelectedEvent(source));
        							}
        							catch (FileNotFoundException fnfe)
        							{
        							}
        						}
        					}.start();
        				}
					}
				});
				
				break;
				
			case STUB:
				eventBus.post(new TransactionSourceSelectedEvent(new StubTransactionSource()));
				break;
				
			default:
				// Do nothing
		}
	}
	
}
