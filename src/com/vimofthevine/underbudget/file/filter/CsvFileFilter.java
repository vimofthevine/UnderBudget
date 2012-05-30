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

package com.vimofthevine.underbudget.file.filter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.swing.filechooser.FileFilter;

/**
 * File filter for CSV files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileFilter extends FileFilter {
	
	/**
	 * Checks if a given file is a valid CSV file
	 * 
	 * @param stream input stream of the file to check
	 * @return true if the file is a valid CSV file
	 */
	public static boolean check(InputStream stream)
	{
		try
		{
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
			return check(reader.readLine(), reader.readLine());
		}
		catch (IOException e) { }
		
		// Not a CSV file
		return false;
	}
	
	/**
	 * Checks if the first line of a file
	 * describes a valid CSV file (presence of commas)
	 * 
	 * @param line1 first line of a potential CSV file
	 * @param line2 second line of a potential CSV file
	 * @return true if a valid CSV file is described
	 */
	public static boolean check(String line1, String line2)
	{
		int numOfFields = line1.split(",").length;
		return (numOfFields > 2) && numOfFields == line2.split(",").length;
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
			boolean isCsvFile = check(stream);
			stream.close();
			return isCsvFile;
		}
		catch (Exception e)
		{
			return false;
		}
    }

	@Override
    public String getDescription()
    {
		return "CSV File";
    }

}
