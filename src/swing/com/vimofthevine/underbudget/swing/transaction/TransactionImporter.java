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

package com.vimofthevine.underbudget.swing.transaction;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.source.TransactionSource;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionsImportedEvent;
import com.vimofthevine.underbudget.swing.transaction.wizard.TransactionSourceSelectionWizard;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionImporter {
	
	private final EventBus eventBus;
	
	private final TransactionSourceSelectionWizard wizard;
	
	public TransactionImporter(EventBus bus)
	{
		eventBus = bus;
		eventBus.register(this);
		wizard = new TransactionSourceSelectionWizard();
	}
	
	@Subscribe
	public void importTransactions(ImportTransactionsEvent event)
	{
		TransactionSource source = wizard.getSource();
		Transaction[] transactions = source.getTransactions(null);
		
		if (transactions != null)
		{
			eventBus.post(new TransactionsImportedEvent(transactions));
		}
	}

}
