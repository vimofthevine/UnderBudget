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

package com.vimofthevine.underbudget.swing.transaction.wizard;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.swing.filechooser.FileFilter;

/**
 * File filter for GnuCash XML files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class GnuCashXmlFileFilter extends FileFilter {
	
	/**
	 * Checks if a given input stream represents
	 * a valid GnuCash file
	 * 
	 * @param stream input stream of the file to check
	 * @return true if the file is a valid GnuCash file
	 */
	boolean check(InputStream stream)
	{
		try
		{
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
			return check(reader.readLine(), reader.readLine());
		}
		catch (IOException e) { }
		
		// Not a GnuCash file
		return false;
	}
	
	/**
	 * Checks if the first two lines of a file
	 * describe a valid GnuCash XML file
	 * 
	 * @param line1 first line of a potential GnuCash file
	 * @param line2 seconds line of a potential GnuCash file
	 * @return true if a valid GnuCash file is described
	 */
	static boolean check(String line1, String line2)
	{
		return line1.startsWith("<?xml")
			&& line2.startsWith("<gnc-v2");
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
			boolean isGnuCashFile = check(stream);
			stream.close();
			return isGnuCashFile;
		}
		catch (Exception e)
		{
			return false;
		}
    }

	@Override
    public String getDescription()
    {
		return "GnuCash XML File";
    }

}