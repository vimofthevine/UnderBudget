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

package com.vimofthevine.underbudget.swing.session.source;

import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * A <code>SourceSummary</code> contains the minimum
 * set of details defining a specific budget source
 * so the source can be re-used at a later time.
 * <p>
 * Source summaries should never contain passwords.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface SourceSummary {
	
	/**
	 * Returns the name of this budget source.
	 * 
	 * @return name of this budget source
	 */
	public String getName();
	
	/**
	 * Returns <code>true</code> if the budget
	 * source requires a password in order to
	 * be reopened.
	 * 
	 * @return <code>true</code> if a password
	 *         is required to reopen, else <code>false</code>
	 */
	public boolean requiresPassword();
	
	/**
	 * Reopens the budget source, optionally using
	 * the given password.
	 * 
	 * @param passkey password
	 * @return reopened budget source, or <code>null</code>
	 *         if the budget source could not be reopened
	 */
	public BudgetSource reopen(String passkey);

	/**
	 * Save this source summary in the user
	 * preferences as the ith session.
	 * 
	 * @param i           session index
	 * @param preferences user preferences
	 */
	public void persist(int i, UserPreferences preferences);
	
}
