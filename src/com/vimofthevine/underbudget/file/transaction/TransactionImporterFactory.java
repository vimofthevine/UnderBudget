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

package com.vimofthevine.underbudget.file.transaction;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.file.filter.CsvFileFilter;
import com.vimofthevine.underbudget.file.filter.GnuCashFileFilter;
import com.vimofthevine.underbudget.file.filter.TransactionFileFilter;

/**
 * Utility for creating an appropriate transaction importer
 * according to the given file type.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionImporterFactory {
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory currencyFactory;
	
	/**
	 * Constructs a new transaction importer factory.
	 * 
	 * @param factory currency factory
	 */
	public TransactionImporterFactory(CurrencyFactory factory)
	{
		currencyFactory = factory;
	}

	/**
	 * Creates a new transaction importer, according to the
	 * type of file if recognized as a supported import source
	 * 
	 * @param stream import content stream
	 * @return TransactionImporter implementation if the given file
	 *          is supported, else null if the given file type is unsupported
	 */
	public TransactionImporter createImporter (InputStream stream)
	{
		String line1;
		String line2;
		
		try
		{
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
			line1 = reader.readLine();
			line2 = reader.readLine();
		}
		catch (IOException ioe)
		{
			return null;
		}
		
		if (TransactionFileFilter.check(line1, line2))
			return new TransactionFileImporter();
		else if (GnuCashFileFilter.check(line1, line2))
			return new GnuCashImporter(currencyFactory);
		else if (CsvFileFilter.check(line1, line2))
			return new CsvImporter(currencyFactory);
		else
			return null;
	}
	
}
