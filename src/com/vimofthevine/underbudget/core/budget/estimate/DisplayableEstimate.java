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

package com.vimofthevine.underbudget.core.budget.estimate;

//import com.vimofthevine.underbudget.budget.estimate.behavior.EstimateBehavior;
//import com.vimofthevine.underbudget.budget.estimate.impact.BalanceImpact;
import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * An estimate that can be displayed to the user.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface DisplayableEstimate {

	/**
	 * Returns a copy of this estimate's name.
	 * 
	 * @return estimate name
	 */
	public String getName();
	
	/**
	 * Returns a copy of this estimate's description.
	 * 
	 * @return estimate description
	 */
	public String getDescription();
	
	/**
	 * Returns a copy of the estimated amount.
	 * 
	 * @return estimate amount
	 */
	public Currency getEstimatedAmount();
	
	/**
	 * Checks if more transactions are expected to be
	 * made against this estimate.
	 * 
	 * @return true if more transactions are expected, else false
	 */
	public boolean moreTransactionsAreExpected();
	
	/**
	 * Returns the balance impact of this estimate.
	 * 
	 * @return estimate impact
	 */
	//public BalanceImpact getImpact();
	
	/**
	 * Returns the behavior of this estimate.
	 * 
	 * @return estimate behavior
	 */
	//public EstimateBehavior getBehavior();
	
}
