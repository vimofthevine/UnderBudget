package com.vimofthevine.underbudget.report.export;

/**
 * Exception thrown when exporting analysis results
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ReportExportException extends Exception {

	/**
	 * Constructor, specifying the exception text
	 * 
	 * @param msg exception detail message
	 */
	public ReportExportException(String msg)
	{
		super(msg);
	}
	
}
