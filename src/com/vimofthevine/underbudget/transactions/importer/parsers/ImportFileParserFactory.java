package com.vimofthevine.underbudget.transactions.importer.parsers;

import java.io.InputStream;

import com.vimofthevine.underbudget.transactions.importer.filter.GnuCashFileFilter;

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
		if (GnuCashFileFilter.check(stream))
			return new GnuCashFileParser();
		else
			return null;
	}

}
