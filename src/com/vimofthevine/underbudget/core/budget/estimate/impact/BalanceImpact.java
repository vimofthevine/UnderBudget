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

package com.vimofthevine.underbudget.core.budget.estimate.impact;

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * The impact an estimate has on the ending balance
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BalanceImpact {
	
	/**
	 * Applies a given amount to a given total.
	 * 
	 * @param amount amount to be applied to the total
	 * @param total  running total amount
	 */
	public void apply(Currency amount, Currency total);

}
