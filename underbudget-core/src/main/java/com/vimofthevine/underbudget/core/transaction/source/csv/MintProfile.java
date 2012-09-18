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

package com.vimofthevine.underbudget.core.transaction.source.csv;

/**
 * Stock CSV profile for exports from mint.com
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public final class MintProfile implements StockCsvProfile {
	
	/**
	 * Transaction fields
	 */
	private final Field[] fields;
	
	/**
	 * Constructs a new stock mint.com
	 * CSV profile.
	 */
	public MintProfile()
	{
		fields = new Field[] {
			Field.DATE,       // date
			Field.PAYEE,      // description
			Field.NONE,       // original description
			Field.AMOUNT,     // amount
			Field.TYPE,       // type
			Field.DEPOSIT,    // category
			Field.WITHDRAWAL, // account name
			Field.NONE,       // labels
			Field.MEMO,       // notes
		};
	}
	
	@Override
	public final String getDescription()
	{
		return "Mint.com CSV export";
	}

	@Override
	public final String getCurrencyCode()
	{
		return "USD";
	}

	@Override
	public final String getDateFormat()
	{
		return "M/dd/yyyy";
	}

	@Override
	public final boolean firstLineContainsHeaders()
	{
		return true;
	}
	
	@Override
	public final int getColumnCount()
	{
		return 9;
	}

	@Override
	public final Field getFieldForColumn(int column)
	{
		if (column < 0 || column >= fields.length)
			return Field.NONE;
		else
			return fields[column];
	}

}
