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
 * A <code>BudgetSourceFactory</code> produces
 * instances of a <code>BudgetSource</code>.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetSourceFactory {
	
	/**
	 * Creates a new budget source instance.
	 * 
	 * @return budget source
	 */
	public BudgetSource create();
	
	/**
	 * Creates a new budget source instance to
	 * hold the given budget.
	 * 
	 * @param budget budget to be saved to the source
	 * @return budget source
	 */
	public BudgetSource create(Budget budget);

}
