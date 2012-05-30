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

package com.vimofthevine.underbudget.file.xml;

import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.convert.Registry;
import org.simpleframework.xml.convert.RegistryStrategy;
import org.simpleframework.xml.core.Persister;
import org.simpleframework.xml.strategy.VisitorStrategy;
import org.simpleframework.xml.stream.Format;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.ledger.transaction.ImportedTransaction;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionList;
import com.vimofthevine.underbudget.file.transaction.TransactionImportException;
import com.vimofthevine.underbudget.file.xml.converter.EmptyStringConverter;
import com.vimofthevine.underbudget.file.xml.converter.FilteredTransactionConverter;
import com.vimofthevine.underbudget.file.xml.strategy.CurrencyVisitor;
import com.vimofthevine.underbudget.file.xml.strategy.TransactionTypeVisitor;

/**
 * Utility for creating and configuration a transaction
 * serializer using the Simple xml framework
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSerializerFactory {

	public static final String PROLOG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";

	/**
	 * Creates a new serializer, configuring it as needed.
	 * 
	 * @return budget serializer
	 */
	public static Serializer createSerializer()
	{
		return createSerializer(null);
	}
	
	/**
	 * Creates a new serializer, configuring it as needed
	 * and filtering transactions to be read by the given
	 * budgeting period.
	 * 
	 * @param period budgeting period to use for filtering
	 * @return budget serializer
	 * @throws TransactionImportException if unable to create
	 *          the serializer
	 */
	public static Serializer createImportSerializer(BudgetingPeriod period)
	{
		return createSerializer(period);
	}
	
	/**
	 * Creates a serializer, configuring it as needed.
	 * 
	 * @param period budgeting period to use for filtering
	 * @return transaction serializer
	 */
	private static Serializer createSerializer(BudgetingPeriod period)
	{
		VisitorStrategy classStrategy = new VisitorStrategy(
			new TransactionTypeVisitor(ImportedTransaction.class.getName()));
		VisitorStrategy currencyStrategy = new VisitorStrategy(new CurrencyVisitor(), classStrategy);
		Registry registry = new Registry();
		RegistryStrategy registryStrategy = new RegistryStrategy(registry, currencyStrategy);
		
		Format format = new Format(PROLOG);
		
		Serializer serializer = new Persister(registryStrategy, format);
		
		try
		{
			if (period != null)
			{
				// Bind converter to transaction list to filter transactions upon import
				registry.bind(TransactionList.class, new FilteredTransactionConverter(period, serializer));
			}
			
    		// Render empty strings instead of null strings
    		registry.bind(String.class, new EmptyStringConverter());
		}
		catch (Exception e) { }
		
		return serializer;
	}
	
}
