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

package com.vimofthevine.underbudget.transactions.importer.parsers;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import com.vimofthevine.underbudget.transactions.importer.filter.GnuCashFileFilter;
import com.vimofthevine.underbudget.transactions.importer.filter.MintCsvFileFilter;

/**
 * Factory class for creating import file parsers
 * according to the detected import file type
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportFileParserFactory {
	
	/**
	 * Creates a new import file parser, according to the
	 * type of file if recognized as a supported import source
	 * 
	 * @param stream import file stream
	 * @return ImportFileParser implementation
	 */
	public static ImportFileParser createParser(InputStream stream)
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
		if (GnuCashFileFilter.check(line1, line2))
			return new GnuCashFileParser();
		else if (MintCsvFileFilter.check(line1))
			return new MintCsvFileParser();
		else
			return null;
	}

}
