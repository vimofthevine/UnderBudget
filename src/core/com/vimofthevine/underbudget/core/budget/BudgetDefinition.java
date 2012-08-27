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

package com.vimofthevine.underbudget.core.budget;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Set of attributes defining a budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetDefinition {

	/**
	 * Returns the name of the budget.
	 * 
	 * @return budget name
	 */
	public String getName();
	
	/**
	 * Returns the initial balance to be
	 * used for ending balance calculations.
	 * 
	 * @return initial balance
	 */
	public Currency getInitialBalance();
	
	/**
	 * Returns the date period over which
	 * to analyze.
	 * 
	 * @return budgeting period
	 */
	public BudgetingPeriod getPeriod();
	
	/**
	 * Returns the currency type, via a currency
	 * factory, to be used for all currency values
	 * in this budget.
	 * 
	 * @return currency factory
	 */
	@Deprecated
	public CurrencyFactory getCurrency();
	
}
