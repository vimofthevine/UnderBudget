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

package com.vimofthevine.underbudget.core.estimate;

import com.vimofthevine.underbudget.core.assignment.ActualFigure;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;

/**
 * An <code>Estimate</code> represents a user's expectations
 * for future incomes or expenses.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Estimate {
	
	/**
	 * Returns the definition of this estimate.
	 * 
	 * @return estimate definition
	 */
	public EstimateDefinition getDefinition();
	
	/**
	 * Checks if this estimate is the root
	 * of the estimate tree.
	 * 
	 * @return <code>true</code> if root, else <code>false</code>
	 */
	public boolean isRoot();
	
	/**
	 * Returns the parent estimate to this estimate.
	 * 
	 * @return parent estimate, else <code>null</code> if root
	 */
	public Estimate getParent();
	
	/**
	 * Returns the number of child estimates to this estimate.
	 * 
	 * @return number of child estimates
	 */
	public int getChildCount();
	
	/**
	 * Returns the index at which the given child
	 * estimate is located.
	 * 
	 * @param child child estimate of interest
	 * @return index at which the child estimate is located,
	 *         else <code>-1</code> if the estimate is not a
	 *         child of this estimate
	 */
	public int indexOf(Estimate child);
	
	/**
	 * Retrieves the child located at the specified index.
	 * 
	 * @param index index of the child to be retrieved
	 * @return child estimate located at the specified index,
	 *         else <code>null</code> if the index is invalid
	 */
	public Estimate getChildAt(int index);
	
	/**
	 * Returns the progress of this estimate as compared
	 * to the given actual figure.
	 * 
	 * @param actual     actual figure to be compared against
	 * @param calculator currency calculator
	 * @return progress of this estimate
	 */
	public EstimateProgress getProgress(ActualFigure actual,
		CurrencyCalculator calculator);
	
	/**
	 * Returns the impact of this estimate to the ending
	 * balances as determined with the given actual figure.
	 * 
	 * @param actual     actual figure to be used
	 * @param calculator currency calculator
	 * @return impact of this estimate to ending balances
	 */
	public BalanceImpact getImpact(ActualFigure actual,
		CurrencyCalculator calculator);
	
}
