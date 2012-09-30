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

import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * Encapsulation of the minimum amount of data
 * necessary to re-open a previously opened session.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface RecentSession {
	
	/**
	 * Returns the name of this recent session.
	 * 
	 * @return recent session name
	 */
	public String getName();
	
	/**
	 * Save this recent session definition in the
	 * user preferences as the ith session.
	 * 
	 * @param i           session index
	 * @param preferences user preferences
	 */
	public void persist(int i, UserPreferences preferences);

}
