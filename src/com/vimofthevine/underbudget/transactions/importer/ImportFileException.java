package com.vimofthevine.underbudget.transactions.importer;

/**
 * Exception thrown when parsing import files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportFileException extends Exception {

	/**
	 * Class constructor, specifying the exception text
	 * 
	 * @param msg exception detail message
	 */
	public ImportFileException(String msg)
	{
		super(msg);
	}
	
}
