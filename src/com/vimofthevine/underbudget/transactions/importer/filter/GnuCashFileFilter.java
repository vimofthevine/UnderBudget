package com.vimofthevine.underbudget.transactions.importer.filter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.swing.filechooser.FileFilter;

/**
 * File filter for GnuCash XML files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GnuCashFileFilter extends FileFilter {
	
	/**
	 * Checks if a given file is a valid GnuCash file
	 * 
	 * @param stream input stream of the file to check
	 * @return true if the file is a valid GnuCash file
	 */
	public static boolean check(InputStream stream)
	{
		// Check if file is a GnuCash XML file
		BufferedReader reader = null;
		
		try
		{
			byte data[] = new byte[5];
			
			// Check if XML file
			if (stream.read(data) == 5
			    && data[0] == '<'
			    && data[1] == '?'
			    && data[2] == 'x'
			    && data[3] == 'm'
			    && data[4] == 'l')
			{
				// Check second line for gnucash element
				reader = new BufferedReader(new InputStreamReader(stream));
				reader.readLine();
				if (reader.readLine().startsWith("<gnc-v2")) {
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
			else if (stream != null)
				stream.close();
		}
		catch (IOException e) { }
		
		// Not a GnuCash file
		return false;
	}

	@Override
    public boolean accept(File file)
    {
		// Accept directories while excluding OS X packages
		if (file.isDirectory())
		{
			if (new File(file, "Contents/PkgInfo").exists())
				return false;
			else
				return true;
		}
		
		try {
			return check(new FileInputStream(file));
		} catch (FileNotFoundException e) {
			return false;
		}
    }

	@Override
    public String getDescription()
    {
		return "GnuCash XML File";
    }

}
