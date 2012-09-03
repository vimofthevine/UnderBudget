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

package com.vimofthevine.underbudget.core.budget.source;

import com.vimofthevine.underbudget.core.budget.Budget;

/**
 * A <code>BudgetSource</code> abstracts the persistence
 * method and location of a user-defined <code>Budget</code>.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetSource {
	
	/**
	 * Returns the budget defined by the specific
	 * source.
	 * 
	 * @return <code>Budget</code> defined by the source
	 * @throws BudgetSourceException if the budget could not
	 *         be retrieved from this budget source
	 */
	public Budget getBudget() throws BudgetSourceException;
	
	/**
	 * Applies any modifications made to the budget,
	 * making them persistent.
	 * 
	 * @throws BudgetSourceException if the budget could not
	 *         be persisted with this budget source
	 */
	public void persist() throws BudgetSourceException;

}
