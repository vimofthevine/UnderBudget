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

import javax.swing.SwingUtilities;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.TransactionDefinition;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionSelectedEvent;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for views that display
 * details about an individual transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportedTransactionDetailViewModel {
	
	/**
	 * Date document model
	 */
	private final SimpleDocument dateDocument;
	
	/**
	 * Payee document model
	 */
	private final SimpleDocument payeeDocument;
	
	/**
	 * Memo document model
	 */
	private final SimpleDocument memoDocument;
	
	/**
	 * Withdrawal account document model
	 */
	private final SimpleDocument withdrawalDocument;
	
	/**
	 * Deposit account document model
	 */
	private final SimpleDocument depositDocument;
	
	/**
	 * Transfer amount document model
	 */
	private final SimpleDocument amountDocument;
	
	/**
	 * Constructs a new imported transaction detail view model.
	 * 
	 * @param bus event bus
	 */
	public ImportedTransactionDetailViewModel(EventBus bus)
	{
		bus.register(this);
		
		dateDocument = new SimpleDocument();
		payeeDocument = new SimpleDocument();
		memoDocument = new SimpleDocument();
		withdrawalDocument = new SimpleDocument();
		depositDocument = new SimpleDocument();
		amountDocument = new SimpleDocument();
	}

	/**
	 * Returns a document representing
	 * the transaction's date posted.
	 * 
	 * @return transaction date document
	 */
	Document getDateDocument()
	{
		return dateDocument;
	}
	
	/**
	 * Returns a document representing
	 * the transaction's payee.
	 * 
	 * @return transaction payee document
	 */
	Document getPayeeDocument()
	{
		return payeeDocument;
	}
	
	/**
	 * Returns a document representing
	 * the transaction's memo.
	 * 
	 * @return transaction memo document
	 */
	Document getMemoDocument()
	{
		return memoDocument;
	}
	
	/**
	 * Returns a document representing
	 * the transaction's amount.
	 * 
	 * @return transaction amount document
	 */
	Document getAmountDocument()
	{
		return amountDocument;
	}
	
	/**
	 * Returns a document representing
	 * the transaction's withdrawal account.
	 * 
	 * @return transaction withdrawal document
	 */
	Document getWithdrawalDocument()
	{
		return withdrawalDocument;
	}
	
	/**
	 * Returns a document representing
	 * the transaction's deposit account.
	 * 
	 * @return transaction deposit document
	 */
	Document getDepositDocument()
	{
		return depositDocument;
	}
	
	/**
	 * Updates the display fields to reflect
	 * the currently selected transaction.
	 * 
	 * @param event transaction selection event
	 */
	@Subscribe
	public void transactionSelected(TransactionSelectedEvent event)
	{
		Transaction transaction = event.getTransaction();
		if (transaction != null)
		{
			final TransactionDefinition definition = transaction.getDefinition();
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					dateDocument.setText(definition.getPostedDate().formatAsString());
					payeeDocument.setText(definition.getPayee());
					memoDocument.setText(definition.getMemo());
					withdrawalDocument.setText(definition.getWithdrawalAccount().getName());
					depositDocument.setText(definition.getDepositAccount().getName());
					amountDocument.setText(definition.getTransferAmount().formatAsString());
				}
			});
		}
	}
	
}
