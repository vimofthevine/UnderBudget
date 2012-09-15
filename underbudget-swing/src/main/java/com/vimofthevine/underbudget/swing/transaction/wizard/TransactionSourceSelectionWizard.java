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
import java.util.Currency;
import java.util.HashMap;
import java.util.Map;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.transaction.source.TransactionSource;
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
	 * Specific source selection wizards
	 */
	private final Map<SourceType, SourceWizard> wizards;
	
	/**
	 * Constructs a new transaction source selection wizard.
	 * 
	 * @param bus      event bus
	 * @param parent   application window
	 * @param currecny currency in use
	 */
	public TransactionSourceSelectionWizard(EventBus bus, Frame parent,
		Currency currency)
	{
		eventBus = bus;
		eventBus.register(this);
		
		window = parent;
		
		wizards = new HashMap<SourceType, SourceWizard>();
		wizards.put(SourceType.GNUCASH_XML, new GnuCashXmlFileWizard());
		wizards.put(SourceType.CSV, new CsvFileWizard());
	}
	
	@Subscribe
	public void selectSource(SelectTransactionSourceEvent event)
	{
		new SourceTypeSelectionDialog(window, this);
	}
	
	/**
	 * Posts the selected source to the event bus to notify
	 * all interested parties of the source selection.
	 * 
	 * @param source selected transaction source
	 */
	void sourceSelected(TransactionSource source)
	{
		eventBus.post(new TransactionSourceSelectedEvent(source));
	}
	
	/**
	 * Initiates the proper wizard for the selected
	 * transaction source type.
	 * 
	 * @param type selected source type
	 */
	void typeSelected(SourceType type)
	{
		SourceWizard wizard = wizards.get(type);
		if (wizard != null)
		{
			wizard.select(window, this);
		}
	}
	
}
