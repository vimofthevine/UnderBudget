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

package com.vimofthevine.underbudget.core.report;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * A balance calculation report records the affect
 * that an estimate has on ending balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BalanceCalculationReport {
	
	/**
	 * Records an estimate's expected amount
	 * and the rationale for the expected amount.
	 * 
	 * @param estimate  estimate whose expected value is being reported
	 * @param expected  expected amount of the estimate
	 * @param rationale rationale for the expected amount. That is,
	 *                   why it is the actual or estimated amount
	 */
	public void record(DisplayableEstimate estimate, Currency expected, ExpectedAmountRationale rationale);

}
