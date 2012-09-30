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

package com.vimofthevine.underbudget.swing.session.recent;

import java.io.File;

import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * Minimum amount of information needed to be able to
 * re-open an AES-encrypted budget file. The user will
 * need to provide the encryption passkey.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RecentAesEncryptedFile implements RecentSession {
	
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
	public RecentAesEncryptedFile(File budgetFile)
	{
		file = budgetFile;
	}

	@Override
	public String getName()
	{
		return file.getName();
	}

	@Override
	public void persist(int i, UserPreferences preferences)
	{
		// TODO Auto-generated method stub

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
		if ( ! (obj instanceof RecentAesEncryptedFile))
			return false;
		
		RecentAesEncryptedFile that = (RecentAesEncryptedFile) obj;
		return this.file.equals(that.file);
	}
	
	@Override
	public String toString()
	{
		return "Recent encrypted file (" + file.getAbsolutePath() + ")";
	}

}
