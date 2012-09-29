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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Currency;
import java.util.Locale;
import java.util.Map.Entry;
import java.util.Properties;

/**
 * A user-defined CSV profile implementation of <code>CsvProfile</code>.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UserCsvProfile implements CsvProfile {
	
	/**
	 * Profile description property key
	 */
	private final static String DESCRIPTION = "Description";
	
	/**
	 * Profile currency property key
	 */
	private final static String CURRENCY = "Currency";
	
	/**
	 * Profile date format property key
	 */
	private final static String DATE_FORMAT = "DateFormat";
	
	/**
	 * Profile first-line-headers property key
	 */
	private final static String HEADERS = "Headers";
	
	/**
	 * Profile column count property key
	 */
	private final static String COLUMNS = "Columns";
	
	/**
	 * Profile field-column property key
	 */
	private final static String FIELD = "Field";
	
	/**
	 * User profile file
	 */
	private final File file;
	
	/**
	 * Profile description
	 */
	private final String description;
	
	/**
	 * Profile currency
	 */
	private final String currency;
	
	/**
	 * Date format
	 */
	private final String dateFormat;
	
	/**
	 * Whether the file will have headers in the first line
	 */
	private final boolean firstLineHeaders;
	
	/**
	 * Number of columns
	 */
	private final int columnCount;
	
	/**
	 * List of transaction fields
	 */
	private final Field[] fields;
	
	/**
	 * Constructs a new CSV profile, with
	 * the definition found in the specified file.
	 * 
	 * @param filename CSV profile filename
	 * @throws IOException if the file could not be read
	 */
	public UserCsvProfile(String filename)
	throws IOException
	{
		file = new File(filename);
		
		Properties properties = new Properties();
		properties.load(new FileInputStream(file));
		
		description = properties.getProperty(DESCRIPTION, "");
		currency = properties.getProperty(CURRENCY,
			Currency.getInstance(Locale.getDefault()).getCurrencyCode());
		dateFormat = properties.getProperty(DATE_FORMAT, "M/dd/yyyy");
		firstLineHeaders = Boolean.parseBoolean(properties.getProperty(HEADERS, "true"));
		columnCount = Integer.parseInt(properties.getProperty(COLUMNS, "0"));
		fields = new Field[columnCount];
		
		for (Entry<Object,Object> entry : properties.entrySet())
		{
			if (((String) entry.getKey()).startsWith(FIELD))
			{
				String column = ((String) entry.getKey()).replace(FIELD, "");
				int index = Integer.parseInt(column);
				Field field = Field.valueOf((String) entry.getValue());
				
				if (index >=0 && index < columnCount)
				{
					fields[index] = field;
				}
			}
		}
	}
	
	/**
	 * Constructs a new CSV profile based on
	 * a given profile, to be saved to the
	 * specified file location.
	 * <p>
	 * To complete the creation of a new profile,
	 * or the updating of an existing profile, the
	 * <code>save()</code> method must be executed.
	 * 
	 * @param filename   CSV profile filename
	 * @param definition CSV profile definition
	 */
	public UserCsvProfile(String filename, CsvProfile definition)
	{
		file = new File(filename);
		
		description = definition.getDescription();
		currency = definition.getCurrencyCode();
		dateFormat = definition.getDateFormat();
		firstLineHeaders = definition.firstLineContainsHeaders();
		columnCount = definition.getColumnCount();
		fields = new Field[columnCount];
		
		for (int i=0; i<columnCount; i++)
		{
			fields[i] = definition.getFieldForColumn(i);
		}
	}
	
	/**
	 * Updates the persistent user-defined CSV profile.
	 * 
	 * @throws IOException if the profile could not be saved
	 */
	public void save() throws IOException
	{
		Properties properties = new Properties();
		
		properties.setProperty(DESCRIPTION, description);
		properties.setProperty(CURRENCY, currency);
		properties.setProperty(DATE_FORMAT, dateFormat);
		properties.setProperty(HEADERS, String.valueOf(firstLineHeaders));
		properties.setProperty(COLUMNS, String.valueOf(columnCount));
		
		for (int i=0; i<fields.length; i++)
		{
			if (fields[i] != null)
			{
				properties.setProperty(FIELD + i, fields[i].toString());
			}
			else
			{
				properties.setProperty(FIELD + i, Field.NONE.toString());
			}
		}
		
		properties.store(new FileOutputStream(file), "User CSV Profile");
	}

	@Override
	public String getDescription()
	{
		return description;
	}

	@Override
	public String getCurrencyCode()
	{
		return currency;
	}

	@Override
	public String getDateFormat()
	{
		return dateFormat;
	}

	@Override
	public boolean firstLineContainsHeaders()
	{
		return firstLineHeaders;
	}
	
	@Override
	public int getColumnCount()
	{
		return columnCount;
	}

	@Override
	public Field getFieldForColumn(int column)
	{
		if (column >=0 && column < columnCount)
		{
			Field field = fields[column];
			return (field == null) ? Field.NONE : field;
		}
		else
			return Field.NONE;
	}
	
	@Override
	public String toString()
	{
		return "User profile, " + description;
	}

}
