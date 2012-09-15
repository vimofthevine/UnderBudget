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

package com.vimofthevine.underbudget.swing.transaction.wizard.csv;

import javax.swing.AbstractListModel;

import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfile;
import com.vimofthevine.underbudget.core.transaction.source.csv.UserCsvProfile;

/**
 * Custom list model for the list of available
 * CSV profiles.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ProfileListModel extends AbstractListModel {
	
	/**
	 * Available CSV profiles
	 */
	private CsvProfile[] profiles;
	
	/**
	 * Constructs a new CSV profile list model
	 * pre-populated with the given list of CSV profiles.
	 * 
	 * @param profiles CSV profiles
	 */
	ProfileListModel(CsvProfile[] profiles)
	{
		this.profiles = profiles;
	}
	
	/**
	 * Updates the list of available CSV profiles.
	 * 
	 * @param profiles new list of CSV profiles
	 */
	void updateList(CsvProfile[] profiles)
	{
		int oldLength = profiles.length;
		this.profiles = profiles;
		this.fireContentsChanged(this, 0,
			Math.max(oldLength, profiles.length));
	}
	
	/**
	 * Returns the CSV profile located at the
	 * given index.
	 * 
	 * @param index list index of interest
	 * @return the CSV profile at the specified index
	 */
	CsvProfile getProfile(int index)
	{
		if (index < 0 || index >= profiles.length)
			return null;
		else
			return profiles[index];
	}

	@Override
	public Object getElementAt(int index)
	{
		if (index < 0 || index >= profiles.length)
			return null;
		else
		{
			CsvProfile profile = profiles[index];
			if (profile instanceof UserCsvProfile)
				return profile.getDescription() + " (custom)";
			else
				return profile.getDescription();
		}
	}

	@Override
	public int getSize()
	{
		return profiles.length;
	}

}
