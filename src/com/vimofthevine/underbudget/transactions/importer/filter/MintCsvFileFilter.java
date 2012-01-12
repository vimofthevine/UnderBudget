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

package com.vimofthevine.underbudget.transactions.importer.filter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.swing.filechooser.FileFilter;

/**
 * File filter for Mint CSV files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class MintCsvFileFilter extends FileFilter {
	
	/**
	 * Checks if a given file is a valid Mint CSV file
	 * 
	 * @param stream input stream of the file to check
	 * @return true if the file is a valid Mint CSV file
	 */
	public static boolean check(InputStream stream)
	{
		try
		{
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
			return check(reader.readLine());
		}
		catch (IOException e) { }
		
		// Not a Mint CSV file
		return false;
	}
	
	/**
	 * Checks if the first line of a file
	 * describes a valid Mint CSV file
	 * 
	 * @param line1 first line of a potential Mint file
	 * @return true if a valid Mint file is described
	 */
	public static boolean check(String line1)
	{
		return line1.startsWith("\"Date\",\"Description\",");
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
		
		try
		{
			InputStream stream = new FileInputStream(file);
			boolean isMintCsvFile = check(stream);
			stream.close();
			return isMintCsvFile;
		}
		catch (Exception e)
		{
			return false;
		}
    }

	@Override
    public String getDescription()
    {
		return "Mint CSV File";
    }

}
