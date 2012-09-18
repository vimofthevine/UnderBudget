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

package com.vimofthevine.underbudget.swing.analysis;

import com.vimofthevine.underbudget.core.balance.EndingBalances;

/**
 * Event generated when ending balances have been calculated.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BalancesCalculatedEvent {
	
	/**
	 * Ending balances
	 */
	private final EndingBalances balances;
	
	/**
	 * Constructs a new balances-calculated event.
	 * 
	 * @param balances ending balances
	 */
	public BalancesCalculatedEvent(EndingBalances balances)
	{
		this.balances = balances;
	}
	
	/**
	 * Returns the ending balances.
	 * 
	 * @return ending balances
	 */
	public EndingBalances getEndingBalances()
	{
		return balances;
	}

}
