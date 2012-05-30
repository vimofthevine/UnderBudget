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

package com.vimofthevine.underbudget.core.budget.estimate.behavior;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;

/**
 * The behavior of an estimate
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateBehavior {
	
	/**
	 * Reports the progress of an estimate to an estimate progress report
	 * 
	 * @param estimate  estimate to be reported
	 * @param estimated estimated value of the estimate
	 * @param actual    actual value of the estimate
	 * @param report    estimate progress report
	 */
	public void report(DisplayableEstimate estimate, Currency estimated,
		Currency actual, EstimateProgressReport report);

}
