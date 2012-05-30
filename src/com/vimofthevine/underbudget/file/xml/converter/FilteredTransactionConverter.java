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

package com.vimofthevine.underbudget.file.xml.converter;

import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.convert.Converter;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.OutputNode;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionList;

/**
 * Converter to create filtered transaction lists.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class FilteredTransactionConverter implements Converter<TransactionList> {
	
	/**
	 * Budgeting period to be used for filtering
	 */
	private final BudgetingPeriod period;
	
	/**
	 * Serializer instance
	 */
	private final Serializer serializer;
	
	/**
	 * Constructs a new converter with a given budgeting period
	 * to be used for filtering and a serializer to read transactions.
	 * 
	 * @param period     budgeting period to be used for filtering
	 * @param serializer serializer to read transactions
	 */
	public FilteredTransactionConverter(BudgetingPeriod period, Serializer serializer)
	{
		this.period = period;
		this.serializer = serializer;
	}

	/**
	 * Creates a filtered transaction list and deserializes all transactions
	 * in the list.
	 */
	@Override
    public TransactionList read(InputNode node) throws Exception
    {
		TransactionList list = new TransactionList(period);
		
		InputNode next = node.getNext();
		while (next != null)
		{
			Transaction transaction = serializer.read(Transaction.class, next);
			list.add(transaction);
			next = node.getNext();
		}
		
		return list;
    }

	@Override
    public void write(OutputNode node, TransactionList value) throws Exception
    {
		// Do nothing
    }

}
