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

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Set of attributes defining an individual estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateDefinition {

	/**
	 * Returns the name of the estimate.
	 * 
	 * @return estimate name
	 */
	public String getName();
	
	/**
	 * Returns the description of the estimate.
	 * 
	 * @return estimate description
	 */
	public String getDescription();
	
	/**
	 * Returns the estimated amount.
	 * 
	 * @return estimated amount
	 */
	public Currency getAmount();
	
	/**
	 * Returns the due-date of the estimate.
	 * 
	 * @return estimate due-date
	 */
	public SimpleDate getDueDate();
	
	/**
	 * Returns the type of the estimate.
	 * 
	 * @return estimate type
	 */
	public EstimateType getType();
	
	/**
	 * Returns the state of the estimate; that is,
	 * whether any more activity is expected to
	 * occur against the estimate.
	 * 
	 * @return <code>true</code> if no more activity
	 *         is expected, else <code>false</code>
	 */
	public boolean isComplete();
	
}
