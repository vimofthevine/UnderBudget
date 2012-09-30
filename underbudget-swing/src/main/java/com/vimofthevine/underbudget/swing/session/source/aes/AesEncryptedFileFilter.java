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

package com.vimofthevine.underbudget.swing.session.source.aes;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Arrays;

import javax.swing.filechooser.FileFilter;

import com.vimofthevine.underbudget.xml.budget.source.AesEncryptedFileSource;

/**
 * File filter for choosing budget files encrypted
 * with AES encryption.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AesEncryptedFileFilter extends FileFilter {
	
	/**
	 * Magic number used to identify AES encrypted files
	 */
	private final byte[] magicNumber;
	
	/**
	 * Constructs a new AES encrypted budget file filter.
	 */
	AesEncryptedFileFilter()
	{
		magicNumber = ByteBuffer.allocate(AesEncryptedFileSource.MAGIC_NUMBER_SIZE)
			.putInt(AesEncryptedFileSource.MAGIC_NUMBER).array();
	}

	@Override
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
		
		// Check if file is an AES-encrypted XML file
		FileInputStream fis = null;
		
		try
		{
			fis = new FileInputStream(f);
			
			// Check for magic number
			byte[] actual = new byte[magicNumber.length];
			
			return (fis.read(actual) == magicNumber.length)
				&& Arrays.equals(magicNumber, actual);
		}
		catch (IOException ioe) { }
		finally
		{
			try
			{
    			if (fis != null)
    			{
    				fis.close();
    			}
			}
			catch (IOException ioe) { }
		}
		
		// Not an encrypted budget file
		return false;
	}

	@Override
	public String getDescription()
	{
		return "AES Encrypted Budget File";
	}

}
