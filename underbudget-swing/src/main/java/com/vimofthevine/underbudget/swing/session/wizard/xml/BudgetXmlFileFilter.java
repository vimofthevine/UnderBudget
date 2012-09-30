package com.vimofthevine.underbudget.swing.session.wizard.xml;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.filechooser.FileFilter;

/**
 * File filter for choosing budget XML files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetXmlFileFilter extends FileFilter {

	/**
	 * Checks if the given file is a valid budget file
	 * 
	 * @param f file to check
	 * @return true if file is a valid budget file
	 */
	public boolean accept(File f)
	{
		// Accept directories while excluding OS X packages
		if (f.isDirectory())
		{
			if (new File(f, "Contents/PkgInfo").exists())
				return false;
			else
				return true;
		}
		
		// Check if file is a budget XML file
		FileInputStream fis = null;
		BufferedReader reader = null;
		
		try
		{
			fis = new FileInputStream(f);
			byte data[] = new byte[5];
			
			// Check if XML file
			if (fis.read(data) == 5
			    && data[0] == '<'
			    && data[1] == '?'
			    && data[2] == 'x'
			    && data[3] == 'm'
			    && data[4] == 'l')
			{
				// Check second line for budget file element
				reader = new BufferedReader(new InputStreamReader(fis));
				reader.readLine();
				if (reader.readLine().startsWith("<budget")) {
					reader.close();
					return true;
				}
			}
		}
		catch (IOException e) { }
		
		// Close the appropriate stream
		try
		{
			if (reader != null)
				reader.close();
			else if (fis != null)
				fis.close();
		}
		catch (IOException e) { }
		
		// Not a budget file
		return false;
	}
	
	public String getDescription()
	{
		return "Budget XML File";
	}
	
}
