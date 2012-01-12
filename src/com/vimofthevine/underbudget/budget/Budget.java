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

package com.vimofthevine.underbudget.budget;

import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Representation of a user-defined budget
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Budget {
	
	/**
	 * Budget meta information
	 */
	public BudgetMeta meta;
	
	/**
	 * Root income estimate
	 */
	public Estimate incomes;
	
	/**
	 * Root expense estimate
	 */
	public Estimate expenses;
	
	/**
	 * Parameterized constructor
	 * 
	 * @param meta     budget meta info
	 * @param incomes  income estimate tree
	 * @param expenses expense estimate tree
	 */
	public Budget(BudgetMeta meta, Estimate incomes, Estimate expenses)
	{
		this.meta     = meta;
		this.incomes  = incomes;
		this.expenses = expenses;
	}

}
