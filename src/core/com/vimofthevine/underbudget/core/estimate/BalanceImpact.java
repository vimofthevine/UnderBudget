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

import com.vimofthevine.underbudget.core.currency.CashCommodity;

/**
 * A <code>BalanceImpact</code> represents the impact
 * a single estimate has on ending balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BalanceImpact {
	
	/**
	 * Returns the impact of the estimate on
	 * the estimated ending balance.
	 * 
	 * @return estimated balance impact
	 */
	public CashCommodity getEstimatedImpact();
	
	/**
	 * Returns the impact of the estimate on
	 * the actual ending balance.
	 * 
	 * @return actual balance impact
	 */
	public CashCommodity getActualImpact();
	
	/**
	 * Returns the impact of the estimate on
	 * the expected ending balance.
	 * 
	 * @return expected balance impact
	 */
	public CashCommodity getExpectedImpact();
	
	/**
	 * Returns any notice that should be
	 * displayed to the user about the
	 * impact of the estimate.
	 * 
	 * @return estimate impact notice
	 */
	public String getNotice();

}
