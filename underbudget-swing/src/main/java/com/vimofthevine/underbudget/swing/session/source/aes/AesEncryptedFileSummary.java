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

import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.source.SourceSummary;
import com.vimofthevine.underbudget.xml.budget.source.AesEncryptedFileSource;

/**
 * Minimum amount of information needed to be able to
 * re-open an AES-encrypted budget file. The user will
 * need to provide the encryption passkey.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AesEncryptedFileSummary implements SourceSummary {
	
	/**
	 * AES-encrypted file type
	 */
	public static final String TYPE = "AES";
	
	/**
	 * File name preference key
	 */
	private static final String FILE = ".File";
	
	/**
	 * Encrypted file
	 */
	private final File file;
	
	/**
	 * Constructs a definition of a recently opened
	 * AES-encrypted budget file.
	 * 
	 * @param file AES-encrypted budget file
	 */
	public AesEncryptedFileSummary(File budgetFile)
	{
		file = budgetFile;
	}
	
	/**
	 * Constructs a definition of a recently opened
	 * AES-encrypted budget file from the given preferences.
	 * 
	 * @param prefix preference key prefix
	 * @param prefs  user preferences
	 * @throws IllegalArgumentException if a required parameter is missing
	 */
	public AesEncryptedFileSummary(String prefix, UserPreferences prefs)
	{
		String fileName = prefs.get(prefix + FILE, "");
		
		if (fileName.equals(""))
			throw new IllegalArgumentException();
		
		file = new File(fileName);
	}
	
	@Override
	public String getName()
	{
		return file.getName();
	}
	
	@Override
	public boolean requiresPassword()
	{
		return true;
	}
	
	@Override
	public BudgetSource reopen(String passkey)
	{
		if (passkey == null || passkey.equals(""))
			return null;
		
		return new AesEncryptedFileSource(file, passkey);
	}

	@Override
	public void persist(String prefix, UserPreferences preferences)
	{
		preferences.set(prefix, TYPE);
		preferences.set(prefix + FILE, file.getAbsolutePath());
	}
	
	@Override
	public int hashCode()
	{
		return 932 * 31 + file.hashCode();
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if ( ! (obj instanceof AesEncryptedFileSummary))
			return false;
		
		AesEncryptedFileSummary that = (AesEncryptedFileSummary) obj;
		return this.file.equals(that.file);
	}
	
	@Override
	public String toString()
	{
		return "AES-encrypted file (" + file.getAbsolutePath() + ")";
	}

}
