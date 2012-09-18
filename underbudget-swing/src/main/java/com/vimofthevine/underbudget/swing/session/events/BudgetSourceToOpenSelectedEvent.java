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

package com.vimofthevine.underbudget.swing.session.events;

import com.vimofthevine.underbudget.core.budget.source.BudgetSource;

/**
 * Event generated when a budget source has
 * been selected.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceToOpenSelectedEvent {
	
	/**
	 * Selected budget source
	 */
	private final BudgetSource source;
	
	/**
	 * Constructs a new budget source
	 * selected event.
	 * 
	 * @param source selected budget source
	 */
	public BudgetSourceToOpenSelectedEvent(BudgetSource source)
	{
		this.source = source;
	}
	
	/**
	 * Returns the selected budget source.
	 * 
	 * @return selected budget source
	 */
	public BudgetSource getSource()
	{
		return source;
	}

}
