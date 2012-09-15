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

import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfile;

/**
 * Event generated when the user has chosen a
 * CSV profile to be used.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ProfileChosenEvent {
	
	/**
	 * The chosen CSV profile
	 */
	private final CsvProfile profile;
	
	/**
	 * Constructs a new profile-chosen event.
	 * 
	 * @param profile the chosen CSV profile
	 */
	ProfileChosenEvent(CsvProfile profile)
	{
		this.profile = profile;
	}
	
	/**
	 * Returns the chosen CSV profile.
	 * 
	 * @return the chosen CSV profile
	 */
	CsvProfile getProfile()
	{
		return profile;
	}

}
