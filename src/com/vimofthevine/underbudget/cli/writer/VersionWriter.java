package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

import com.vimofthevine.underbudget.Application;

/**
 * Writes application version and info to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class VersionWriter {
	
	/**
	 * Writes application version to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println(Application.TITLE + " " + Application.VERSION + " (" + Application.CODENAME + ")");
		stream.println("Copyright (C) " + Application.COPY_YEAR + " " + Application.COPY_AUTHOR);
		stream.println(Application.WEBSITE);
	}

}
